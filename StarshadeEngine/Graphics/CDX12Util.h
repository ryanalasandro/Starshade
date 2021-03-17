//-------------------------------------------------------------------------------------------------
//
// Copyright (c) Ryan Alasandro
//
// Static Library: Starshade Engine
//
// File: Graphics/CDX12Util.h
//
//-------------------------------------------------------------------------------------------------

#ifndef CDX12UTIL_H
#define CDX12UTIL_H

#include <d3d12.h>

namespace Graphics {
	/*
		D3D12_RESOURCE_BARRIER barrier = { };
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = m_ppRenderTargetList[m_bufferIndex];
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	*/

	D3D12_RESOURCE_BARRIER CreateTransitionBarrier(
		ID3D12Resource* pResource,
		D3D12_RESOURCE_STATES stateBefore,
		D3D12_RESOURCE_STATES stateAfter,
		D3D12_RESOURCE_BARRIER_FLAGS flags = D3D12_RESOURCE_BARRIER_FLAG_NONE,
		UINT subResource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES)
	{
		D3D12_RESOURCE_BARRIER barrier = { };
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = flags;
		barrier.Transition.pResource = pResource;
		barrier.Transition.StateBefore = stateBefore;
		barrier.Transition.StateAfter = stateAfter;
		barrier.Transition.Subresource = subResource;
		return barrier;
	}
};

#endif
