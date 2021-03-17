//-------------------------------------------------------------------------------------------------
//
// Copyright (c) Ryan Alasandro
//
// Static Library: Starshade Engine
//
// File: Graphics/CDX12Graphics.hpp
//
//-------------------------------------------------------------------------------------------------

//#include "CDX12Graphics.h"
#include "CDX12Util.h"
#include "../Application/CPanelWin.h"
#include "../Utilities/CEngineError.h"
#include "../Utilities/CMemoryFree.h"
#include <wrl.h>

namespace Graphics {
	CDX12Graphics::CDX12Graphics() :
		m_bufferIndex(0),
		m_rtvDescSize(0),
		m_aspectRatio(0.0f),
		m_pFactory(nullptr),
		m_pDevice(nullptr),
		m_pCommandQueue(nullptr),
		m_pSwapChain(nullptr),
		m_pRTVHeap(nullptr),
		m_ppRenderTargetList(nullptr),
		m_ppCommandAllocatorList(nullptr),
		m_pCommandList(nullptr),
		m_pFenceValueList(nullptr),
		m_hFenceEvent(nullptr),
		m_pFence(nullptr) {
	}

	CDX12Graphics::~CDX12Graphics() {
	}

	void CDX12Graphics::Initialize() {
		using namespace Microsoft::WRL;

		u32 factoryFlags = 0;

#ifdef _DEBUG
		{ // Enable debug layer in debug build.
			ComPtr<ID3D12Debug> pDebugController;
			AssertHR(D3D12GetDebugInterface(IID_PPV_ARGS(&pDebugController)));
			pDebugController->EnableDebugLayer();
			factoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
		}
#endif

		// Create dxgi factory.
		AssertHRCode(CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(&m_pFactory)),
			Err::GRAPHICS_DX12_FAILED_TO_CREATE_DXGI_FACTORY);

		{ // Get adapters and create device for the first valid one.
			const u32 adapterIndex = PopulateAdapterList();
			AssertCode(adapterIndex != ~0, Err::GRAPHICS_DX12_FAILED_TO_GET_VALID_ADAPTER);

			ComPtr<IDXGIAdapter1> pHardwareAdapter;
			AssertHR(m_pFactory->EnumAdapters1(adapterIndex, &pHardwareAdapter));

			// Create dx12 device with found adapter.
			AssertHRCode(D3D12CreateDevice(pHardwareAdapter.Get(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&m_pDevice)), 
				Err::GRAPHICS_DX12_FAILED_TO_CREATE_DEVICE);
			m_pDevice->SetName(L"DX12 Device");
		}

		{ // Check for tearing support.
			BOOL allowTearing = FALSE;
			if(SUCCEEDED(m_pFactory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(allowTearing)))) {
				m_bTearingSupport = allowTearing;
			} else {
				m_bTearingSupport = false;
			}
		}

		{ // Check for raytracing support.
			D3D12_FEATURE_DATA_D3D12_OPTIONS5 options5 = { };
			if(SUCCEEDED(m_pDevice->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5, &options5, sizeof(options5)))) {
				m_bRaytracingSupport = options5.RaytracingTier >= D3D12_RAYTRACING_TIER_1_0;
			} else {
				m_bRaytracingSupport = false;
			}
		}

		auto pWinPanel = dynamic_cast<const App::CPanelWin*>(m_pPanel);

		// Currently alt-enter fullscreen transitioning isn't supported.
		AssertHR(m_pFactory->MakeWindowAssociation(pWinPanel->GetWindowHandle(), DXGI_MWA_NO_ALT_ENTER));
		
		{ // Describe and create the command queue.
			D3D12_COMMAND_QUEUE_DESC queueDesc = { };
			queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
			AssertHR(m_pDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_pCommandQueue)));
			m_pCommandQueue->SetName(L"DX12 Command Queue");
		}

		{ // Describe and create render target view heap.
			D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
			rtvHeapDesc.NumDescriptors = m_bufferCount;
			rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			AssertHR(m_pDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_pRTVHeap)));
			m_rtvDescSize = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		}

		InitSwapChain();
		InitBuffers();
		InitBufferDepends();

		{ // Create panel command list.
			AssertHR(m_pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, 
				m_ppCommandAllocatorList[m_bufferIndex], nullptr, IID_PPV_ARGS(&m_pCommandList)));
			m_pCommandList->SetName(L"DX12 Command List");
		}

		//---------------------------------------------------------------------------------------------
		// Load Assets.
		//---------------------------------------------------------------------------------------------

		// LoadAssets();
		
		//---------------------------------------------------------------------------------------------
		// *** END --- Load Assets.
		//---------------------------------------------------------------------------------------------
			
		{ // Close the command list and execute it for assets that have pushed things to it.
			AssertHR(m_pCommandList->Close());
			ID3D12CommandList* ppCommandList[] = { m_pCommandList };
			m_pCommandQueue->ExecuteCommandLists(_countof(ppCommandList), ppCommandList);
		}

		{ // Create synchronization objects.
			AssertHR(m_pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_pFence)));
			m_pFenceValueList[m_bufferIndex] = 1;
			m_hFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
			if(m_hFenceEvent == nullptr) {
				AssertHR(HRESULT_FROM_WIN32(GetLastError()));
			}

			m_pFence->SetName(L"DX12 Fence");
			WaitForGPU();
		}
	}

	void CDX12Graphics::InitSwapChain() {
		using namespace Microsoft::WRL;
		auto pWinPanel = dynamic_cast<const App::CPanelWin*>(m_pPanel);

		// Create swapchain.
		m_swapChainDesc = { };
		m_swapChainDesc.BufferCount = m_bufferCount;
		m_swapChainDesc.Width = pWinPanel->GetBounds().w;
		m_swapChainDesc.Height = pWinPanel->GetBounds().h;
		m_swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		m_swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		m_swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		m_swapChainDesc.SampleDesc.Count = 1;
		m_swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
		m_swapChainDesc.Flags = m_bTearingSupport ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;

		ComPtr<IDXGISwapChain1> pSwapChain;
		AssertHRCode(m_pFactory->CreateSwapChainForHwnd(
			m_pCommandQueue,
			pWinPanel->GetWindowHandle(),
			&m_swapChainDesc,
			nullptr,
			nullptr,
			&pSwapChain
		), Err::GRAPHICS_DX12_FAILED_TO_CREATE_SWAPCHAIN);
		
		// Convert created swap chain to swap chain 3.
		AssertHRCode(pSwapChain->QueryInterface(__uuidof(IDXGISwapChain3), reinterpret_cast<void**>(&m_pSwapChain)),
			Err::GRAPHICS_DX12_FAILED_TO_CONVERT_SWAPCHAIN);
	}

	// Method for creating target render buffers.
	void CDX12Graphics::InitBuffers() {
		// Allocate memory. Use dynamic memory seeing as buffer count might change while in game.
		m_ppRenderTargetList = new ID3D12Resource*[m_bufferCount];

		for(u8 i = 0; i < m_bufferCount; ++i) {
			m_ppRenderTargetList[i] = nullptr;
		}

		{ // Create render targets.
			D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_pRTVHeap->GetCPUDescriptorHandleForHeapStart();

			// Create render target views with swap chain buffers.
			for(u8 i = 0; i < m_bufferCount; ++i) {
				AssertHR(m_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&m_ppRenderTargetList[i])));
				m_pDevice->CreateRenderTargetView(m_ppRenderTargetList[i], nullptr, rtvHandle);
				rtvHandle.ptr += static_cast<size_t>(m_rtvDescSize);
			}
		}

		{ // Setup viewport, scissor rect and aspect ratio based on set panel.
			m_viewport.TopLeftX = 0.0f;
			m_viewport.TopLeftY = 0.0f;
			m_viewport.MinDepth = D3D12_MIN_DEPTH;
			m_viewport.MaxDepth = D3D12_MAX_DEPTH;
			m_viewport.Width = FLOAT(m_pPanel->GetBounds().w);
			m_viewport.Height = FLOAT(m_pPanel->GetBounds().h);

			m_scissorRect.left = 0;
			m_scissorRect.top = 0;
			m_scissorRect.right = LONG(m_pPanel->GetBounds().w);
			m_scissorRect.bottom = LONG(m_pPanel->GetBounds().h);

			m_aspectRatio = m_viewport.Width / m_viewport.Height;
		}
	}

	// Method for initializing objects dependent on total buffer count.
	void CDX12Graphics::InitBufferDepends() {
		// Allocate memory. Use dynamic memory seeing as buffer count might change while in game.
		m_ppCommandAllocatorList = new ID3D12CommandAllocator*[m_bufferCount];
		m_pFenceValueList = new u64[m_bufferCount];

		for(u8 i = 0; i < m_bufferCount; ++i) {
			m_ppCommandAllocatorList[i] = nullptr;
			m_pFenceValueList[i] = 0;
		}
		
		for(u8 i = 0; i < m_bufferCount; ++i) {
			// Create panel command allocator.
			AssertHR(m_pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, 
				IID_PPV_ARGS(&m_ppCommandAllocatorList[i])));
			m_ppCommandAllocatorList[i]->SetName(L"DX12 Command Allocator");
		}
	}

	//-----------------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------------

	// Method for rendering a single frame.
	void CDX12Graphics::Render(const std::function<void()>& RenderScene) {
		AssertHR(m_ppCommandAllocatorList[m_bufferIndex]->Reset());
		AssertHR(m_pCommandList->Reset(m_ppCommandAllocatorList[m_bufferIndex], nullptr));

		// Indicate that the back buffer will be used as a render target.
		D3D12_RESOURCE_BARRIER barrier = CreateTransitionBarrier(m_ppRenderTargetList[m_bufferIndex], 
			D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

		m_pCommandList->ResourceBarrier(1, &barrier);
		
		// Set frame render target and depth stencil view.
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_pRTVHeap->GetCPUDescriptorHandleForHeapStart();
		rtvHandle.ptr += static_cast<size_t>(m_bufferIndex * m_rtvDescSize);
		m_pCommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

		// Set viewports and scissor rects.
		m_pCommandList->RSSetViewports(1, &m_viewport);
		m_pCommandList->RSSetScissorRects(1, &m_scissorRect);

		// Clear frame render target and depth stencil view.
		m_pCommandList->ClearRenderTargetView(rtvHandle, m_bgColor, 0, nullptr);

		// Set descriptor heaps.
		//m_heapManager.SetToCmdList();
		
		RenderScene();

		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

		// Indicate that the back buffer will now be used to present.
		m_pCommandList->ResourceBarrier(1, &barrier);
		
		AssertHR(m_pCommandList->Close());

		// Execute the command list.
		ID3D12CommandList* ppCommandLists[] = { m_pCommandList };
		m_pCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

		{ // Present.
			u32 presentFlags = 0;//m_bTearingSupport && m_vBlanks == VBlanks::_0 ? DXGI_PRESENT_ALLOW_TEARING : 0;
			AssertHR(m_pSwapChain->Present(u32(m_vBlanks), presentFlags));
		}

		MoveToNextFrame();
	}
	
	// Method for adjusting swap chain and buffers to panel resize.
	void CDX12Graphics::AdjustToResize() {
		if(m_pSwapChain) {
			WaitForGPU();
			
			for(u8 i = 0; i < m_bufferCount; ++i) {
				SAFE_RELEASE(&m_ppRenderTargetList[i]);
				m_pFenceValueList[i] = m_pFenceValueList[m_bufferIndex];
			} SAFE_DELETE_ARRAY(&m_ppRenderTargetList);

			auto pWinPanel = dynamic_cast<const App::CPanelWin*>(m_pPanel);

			// Update swapchain description.
			m_swapChainDesc.Width = pWinPanel->GetBounds().w;
			m_swapChainDesc.Height = pWinPanel->GetBounds().h;

			SetFullscreen(m_pPanel->GetScreenMode() == App::PANEL_SCREEN_MODE::PSM_FULLSCREEN);
			AssertHR(m_pSwapChain->ResizeBuffers(m_swapChainDesc.BufferCount, m_swapChainDesc.Width, 
				m_swapChainDesc.Height, m_swapChainDesc.Format, m_swapChainDesc.Flags));
			m_bufferIndex = m_pSwapChain->GetCurrentBackBufferIndex();
			
			InitBuffers();

			AssertHR(m_ppCommandAllocatorList[m_bufferIndex]->Reset());
			AssertHR(m_pCommandList->Reset(m_ppCommandAllocatorList[m_bufferIndex], nullptr));
		}
	}

	// Method for finalizing a panel resize. This executes on any commands sent during the resize.
	void CDX12Graphics::FinalizeResize() {
		if(m_pSwapChain) {
			AssertHR(m_pCommandList->Close());

			// Execute the command list.
			ID3D12CommandList* ppCommandLists[] = { m_pCommandList };
			m_pCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

			MoveToNextFrame();
		}
	}

	// Method for setting fullscreen state.
	void CDX12Graphics::SetFullscreen(bool bFullscreen) {
		if(m_bTearingSupport) return;

		BOOL fullscreen;
		AssertHR(m_pSwapChain->GetFullscreenState(&fullscreen, nullptr));
		if(bool(fullscreen) == bFullscreen) return;

		if(bFullscreen) {
			AssertHR(m_pSwapChain->SetFullscreenState(bFullscreen, nullptr));
		} else {
			AssertHR(m_pSwapChain->SetFullscreenState(bFullscreen, nullptr));
		}
	}

	// Method for cleaning up any resources allocated to this object.
	void CDX12Graphics::Release() {
		WaitForGPU();

		if(!m_bTearingSupport) {
			AssertHR(m_pSwapChain->SetFullscreenState(FALSE, nullptr));
		}

		CloseHandle(m_hFenceEvent);
		SAFE_RELEASE(&m_pFence);
		SAFE_DELETE_ARRAY(&m_pFenceValueList);
		m_bufferIndex = 0;

		SAFE_RELEASE(&m_pCommandList);

		for(u8 i = 0; i < m_bufferCount; i++) {
			SAFE_RELEASE(&m_ppCommandAllocatorList[i]);
			SAFE_RELEASE(&m_ppRenderTargetList[i]);
		}

		SAFE_DELETE_ARRAY(&m_ppCommandAllocatorList);
		SAFE_DELETE_ARRAY(&m_ppRenderTargetList);

		SAFE_RELEASE(&m_pSwapChain);
		SAFE_RELEASE(&m_pRTVHeap);
		SAFE_RELEASE(&m_pCommandQueue);

		SAFE_RELEASE(&m_pDevice);
		SAFE_RELEASE(&m_pFactory);

		CGraphicsAPI::Release();
	}

	//-----------------------------------------------------------------------------------------------
	// Graphics API utility methods.
	//-----------------------------------------------------------------------------------------------

	// Method for populating the valid video adapter list.
	u32 CDX12Graphics::PopulateAdapterList() {
		using namespace Microsoft::WRL;

		m_adapterDataList.clear();
		
		u32 firstIndexSupp = ~0;
		ComPtr<IDXGIAdapter1> pHardwareAdapter;
		DXGI_ADAPTER_DESC1 adapterDesc;
		for(u32 adapterIndex = 0; m_pFactory->EnumAdapters1(adapterIndex, &pHardwareAdapter) != DXGI_ERROR_NOT_FOUND; ++adapterIndex) {
			AssertHR(pHardwareAdapter->GetDesc1(&adapterDesc));

			// Create adapter data for adapter data list.
			ADAPTER_DATA data = { };
			wcscpy_s(data.description, adapterDesc.Description);
			data.bSupported = false;
			data.index = adapterIndex;
			data.vendorId = adapterDesc.VendorId;
			data.deviceId = adapterDesc.DeviceId;
			data.subSysId = adapterDesc.SubSysId;
			data.revision = adapterDesc.Revision;
			data.dedicatedVideoMemory = adapterDesc.DedicatedVideoMemory;
			data.dedicatedSystemMemory = adapterDesc.DedicatedSystemMemory;
			data.sharedSystemMemory = adapterDesc.SharedSystemMemory;
			
			m_adapterDataList.push_back(data);
			
			if(adapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
				// Adapter is software based, which is not considered supported.
				continue;
			} 
			
			if(SUCCEEDED(D3D12CreateDevice(pHardwareAdapter.Get(), D3D_FEATURE_LEVEL_12_0, __uuidof(ID3D12Device), nullptr))) {
				// Adapter is supported. If it is the first on to be, store its index so that it can be returned.
				m_adapterDataList[m_adapterDataList.size() - 1].bSupported = true;
				if(firstIndexSupp == ~0) {
					firstIndexSupp = adapterIndex;
				}
			}
		}

		return firstIndexSupp;
	}
	
	//-----------------------------------------------------------------------------------------------
	// Fence methods.
	//-----------------------------------------------------------------------------------------------

	void CDX12Graphics::WaitForGPU() {
		if(m_pFence == nullptr || m_pFence->GetCompletedValue() >= m_pFenceValueList[m_bufferIndex]) {
			return;
		}

		AssertHR(m_pCommandQueue->Signal(m_pFence, m_pFenceValueList[m_bufferIndex]));
		
		AssertHR(m_pFence->SetEventOnCompletion(m_pFenceValueList[m_bufferIndex], m_hFenceEvent));
		WaitForSingleObjectEx(m_hFenceEvent, INFINITE, FALSE);

		m_pFenceValueList[m_bufferIndex]++;
	}

	void CDX12Graphics::MoveToNextFrame() {
		// Signal and increment the fence value.
		const u64 currentFenceValue = m_pFenceValueList[m_bufferIndex];
		AssertHR(m_pCommandQueue->Signal(m_pFence, currentFenceValue));
		
		m_bufferIndex = m_pSwapChain->GetCurrentBackBufferIndex();

		// Wait until the previous frame is finished.
		if(m_pFence->GetCompletedValue() < m_pFenceValueList[m_bufferIndex]) {
			AssertHR(m_pFence->SetEventOnCompletion(m_pFenceValueList[m_bufferIndex], m_hFenceEvent));
			WaitForSingleObjectEx(m_hFenceEvent, INFINITE, FALSE);
		}

		m_pFenceValueList[m_bufferIndex] = currentFenceValue + 1;
	}
};
