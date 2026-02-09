/*************************************************************************
*
* ADOBE CONFIDENTIAL
*
* Copyright 2017 Adobe
*
* All Rights Reserved.
*
* NOTICE: Adobe permits you to use, modify, and distribute this file in
* accordance with the terms of the Adobe license agreement accompanying
* it. If you have received this file from a source other than Adobe,
* then your use, modification, or distribution of it requires the prior
* written permission of Adobe.
*
**************************************************************************/

#pragma once

#include "AIErrorHandler.h"
#include <memory>
#include <mutex>
#include <atomic>

/** @file IAIScopedCache.hpp */

namespace ai
{

	/**
		BooleanAttributeCache:	A class to store boolean attributes.
								Attributes are generally specified via some
								enumeration. Storage of each attribute value
								inside the cache is in a single bit. Additional
								storage contains another bit per attribute to
								indicate initialization status.
	
		Sample usage:
	
		enum class DocumentState : ai::uint8
		{ kUnsaved = 0, kInIsolationMode = 1, ... kUsingSpotColor = 30 };
	
		using BitStorage = ai::uint32;	// Large enough to hold 1 bit for each
										// value in enumeration above.
	
		using DocAttrCache = BooleanAttributeCache <DocumentState, BitStorage>;
	
		using DocAttrCacheScopeManager = ai::CacheScopeManager <DocAttrCache>;
		DocAttrCacheScopeManager cacheScope;
	
		...
	
		bool docIsUnsaved{ false }, docIsInIsolationMode{ false };
	
		if (DocAttrCacheScopeManager::IsCacheValid())
		{
			if (DocAttrCacheScopeManager::GetCache()->GetValue(DocumentState::kUnsaved, docIsUnsaved) == false)
			{
				// Compute and cache value
				docIsUnsaved = ComputeDocIsUnsaved();
				MyCacheScopeManager::GetCache()->SetValue(DocumentState::kUnsaved, docIsUnsaved);
			}
	
			if (DocAttrCacheScopeManager::GetCache()->GetValue(DocumentState::kInIsolationMode, docIsInIsolationMode) == false)
			{
				// Compute and cache value
				docIsInIsolationMode = ComputeDocIsInIsolationMode();
				MyCacheScopeManager::GetCache()->SetValue(DocumentState::kInIsolationMode, docIsInIsolationMode);
			}
		}
		else
		{
			// Compute
			docIsUnsaved = ComputeDocIsUnsaved();
			docIsInIsolationMode = ComputeDocIsInIsolationMode();
	
			// Do not store as cache is invalid
		}
	
		// Use values
		Print(docIsUnsaved);
		Print(docIsInIsolationMode);
	*/
	template <typename Attribute_t, typename Bit_t>
	class BooleanAttributeCache
	{
	public:
		// If cached value is found, retrieves it and returns true.
		// Otherwise returns false.
		bool GetValue(Attribute_t inAttr, bool& outValue) const AINOEXCEPT
		{
			const auto initialized = IsInitialized(inAttr);
			if (initialized)
			{
				outValue = ((fAttrBits & AttributeToBits(inAttr)) ? true : false);
			}
			return initialized;
		}
		void SetValue(Attribute_t inAttr, const bool inValue) AINOEXCEPT
		{
			const auto flagBit = AttributeToBits(inAttr);
			if (inValue)
			{
				fAttrBits |= flagBit;
			}
			else
			{
				fAttrBits &= ~flagBit;
			}
			fInitBits |= flagBit;
		}
		void Clear() AINOEXCEPT
		{
			fInitBits = fAttrBits = 0;
		}

	private:
		bool IsInitialized(Attribute_t inAttr) const AINOEXCEPT
		{
			return ((fInitBits & AttributeToBits(inAttr)) ? true : false);
		}
		Bit_t AttributeToBits(Attribute_t inAttr) const AINOEXCEPT
		{
			return (1 << static_cast<Bit_t>(inAttr));
		}

	private:
		Bit_t fAttrBits{ 0 }, fInitBits{ 0 };
	};

	// Forward declaration of base implementation
	template <typename Cache_t, bool IsThreadLocal>
	class CacheScopeManagerImpl;

	// Thread-local specialization
	template <typename Cache_t>
	class CacheScopeManagerImpl<Cache_t, true> {
	protected:
		static std::unique_ptr<Cache_t>& Cache() {
			static thread_local std::unique_ptr<Cache_t> sCache;
			return sCache;
		}

		static size_t& CacheRefCount() AINOEXCEPT {
			static thread_local size_t sCacheRefCount{ 0 };
			return sCacheRefCount;
		}

		static size_t& CacheSuppressCount() AINOEXCEPT {
			static thread_local size_t sCacheSuppressCount{ 0 };
			return sCacheSuppressCount;
		}
	};

	// Global (non-thread-local) specialization with mutex protection
	template <typename Cache_t>
	class CacheScopeManagerImpl<Cache_t, false> {
	protected:
		static std::mutex& GetMutex() {
			static std::mutex sMutex;
			return sMutex;
		}

		static std::unique_ptr<Cache_t>& Cache() {
			static std::unique_ptr<Cache_t> sCache;
			return sCache;
		}

        static size_t& CacheRefCount() AINOEXCEPT {
            static size_t sCacheRefCount{ 0 };
            return sCacheRefCount;
        }

        static size_t& CacheSuppressCount() AINOEXCEPT {
            static  size_t sCacheSuppressCount{ 0 };
            return sCacheSuppressCount;
        }
	};

	// Modified CacheScopeManager for thread-safe global cache
	template <typename Cache_t, bool IsThreadLocal = true>
	class CacheScopeManager : private CacheScopeManagerImpl<Cache_t, IsThreadLocal>
	{
		using BaseImpl = CacheScopeManagerImpl<Cache_t, IsThreadLocal>;
	public:
		CacheScopeManager() : CacheScopeManager{ false } {}
		virtual ~CacheScopeManager()
		{
			try
			{
                std::unique_ptr<std::lock_guard<std::mutex>> lock =  nullptr;
                if constexpr (!IsThreadLocal)
                {
                    lock = std::make_unique<std::lock_guard<std::mutex>>(BaseImpl::GetMutex());
                }
        		if (fSuppressor)
				{
					if (BaseImpl::CacheRefCount() != 0)
					{
                        if (BaseImpl::CacheSuppressCount())
                        {
                            --BaseImpl::CacheSuppressCount();
                            if (BaseImpl::CacheSuppressCount() == 0)
                            {
                                BaseImpl::Cache() = std::make_unique<Cache_t>();
                            }
                            
                        }
					}
				}
				else
				{
					if (BaseImpl::CacheSuppressCount() == 0)
					{
                        if ( BaseImpl::CacheRefCount())
                        {
                            --BaseImpl::CacheRefCount();
                            if (BaseImpl::CacheRefCount() == 0)
                            {
                                BaseImpl::Cache().reset();
                            }
                        }
						
					}
				}
			}
			AI_CATCH_ASSERT_NO_RETURN
		}

		static bool IsCacheValid() AINOEXCEPT
		{
			if constexpr (!IsThreadLocal) {
				std::lock_guard<std::mutex> lock(BaseImpl::GetMutex());
				return GetCacheImpl() != nullptr;
			} else {
				return GetCacheImpl() != nullptr;
			}
		}

		static Cache_t* GetCache() AINOEXCEPT
		{
			if constexpr (!IsThreadLocal) {
				std::lock_guard<std::mutex> lock(BaseImpl::GetMutex());
				return GetCacheImpl();
			} else {
				return GetCacheImpl();
			}
		}

	protected:
		CacheScopeManager(bool suppressor) : fSuppressor{ suppressor }
		{
            std::unique_ptr<std::lock_guard<std::mutex>> lock =  nullptr;
            if constexpr (!IsThreadLocal)
            {
                lock = std::make_unique<std::lock_guard<std::mutex>>(BaseImpl::GetMutex());
            }
			if (fSuppressor)
			{
				if (BaseImpl::CacheRefCount() != 0)
				{
                    if (BaseImpl::CacheSuppressCount() == 0)
                    {
                        BaseImpl::Cache().reset();
                    }
					++BaseImpl::CacheSuppressCount();
				}
			}
			else
			{
				if (BaseImpl::CacheSuppressCount() == 0)
				{
                    if (BaseImpl::CacheRefCount() == 0)
                    {
                        BaseImpl::Cache() = std::make_unique<Cache_t>();
                    }
					++BaseImpl::CacheRefCount();
				}
			}
		}

	private:
		static Cache_t* GetCacheImpl() AINOEXCEPT {
			return BaseImpl::Cache().get();
		}

		const bool fSuppressor;
	};

	/**
		CacheScopeSuppressor:	Prevents caching in a scope.
								This invalidates cache which may have been
								created by CacheScopeManager of the same type
								and prevents further caching of that type even
								if new instances of CacheScopeManager are
								created.
								CacheScopeSuppressor instances can be safely
								nested.
								When the last CacheScopeSuppressor goes out of
								scope and we have at least one CacheScopeManager
								still in scope, caching is resumed but the cache
								is empty and is not restored to the old value
								before the first CacheScopeSuppressor came in
								scope.
	*/	
	template <typename Cache_t, bool IsThreadLocal = true>
	class CacheScopeSuppressor : public CacheScopeManager <Cache_t, IsThreadLocal>
	{
	public:
		CacheScopeSuppressor() : CacheScopeManager <Cache_t, IsThreadLocal> { true } {}
	};
}	// namespace ai
