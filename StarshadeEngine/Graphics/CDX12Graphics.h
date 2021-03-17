//-------------------------------------------------------------------------------------------------
//
// Copyright (c) Ryan Alasandro
//
// Static Library: Starshade Engine
//
// File: Graphics/CDX12Graphics.h
//
//-------------------------------------------------------------------------------------------------

#ifndef CDX12GRAPHICS_H
#define CDX12GRAPHICS_H

#include "CGraphicsAPI.h"
#include <array>
#include <d3d12.h>
#include <dxgi1_6.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

namespace Graphics {
	class CDX12Graphics : public CGraphicsAPI {
	public:
		CDX12Graphics();
		~CDX12Graphics();
		
	protected:
		void Initialize() final;
		void InitSwapChain();
		void InitBuffers();
		void InitBufferDepends();

		void Render(const std::function<void()>& RenderScene) final;
		void AdjustToResize() final;
		void FinalizeResize() final;
		void SetFullscreen(bool bFullscreen) final;
		void Release() final;

		void WaitForGPU();
		void MoveToNextFrame();

		u32 PopulateAdapterList();

	private:
		u8 m_bufferIndex;
		u32 m_rtvDescSize;

		float m_aspectRatio;
		D3D12_VIEWPORT m_viewport;
		D3D12_RECT m_scissorRect;

		DXGI_SWAP_CHAIN_DESC1 m_swapChainDesc;

		IDXGIFactory6* m_pFactory;
		ID3D12Device5* m_pDevice;
		ID3D12CommandQueue* m_pCommandQueue;
		IDXGISwapChain3* m_pSwapChain;
		ID3D12DescriptorHeap* m_pRTVHeap;
		ID3D12Resource** m_ppRenderTargetList;
		ID3D12CommandAllocator** m_ppCommandAllocatorList;
		ID3D12GraphicsCommandList4* m_pCommandList;

		u64* m_pFenceValueList;
		HANDLE m_hFenceEvent;
		ID3D12Fence* m_pFence;
	};
};

#include "CDX12Graphics.hpp"

#endif
