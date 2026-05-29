#pragma once
#include <DX3D/Core/Logger.h>
#include <d3d11.h>

namespace dx3d
{
	namespace GraphicsLogUtils
	{
		inline void CheckShaderCompile(Logger& logger, HRESULT hr, ID3DBlob* errorBlob)
		{
			auto errorMessage = errorBlob ? static_cast<const char*>(errorBlob->GetBufferPointer()) : nullptr;

			if (FAILED(hr))
			{
				if (errorMessage)
				{
					DX3DLogThrow(logger, std::runtime_error, Logger::LogLevel::Error, errorMessage);
				}

				else
				{
					DX3DLogThrow(logger, std::runtime_error, Logger::LogLevel::Error, 
						"Shader compilation has failed!");
				}
			}

			if (errorMessage)
			{
				DX3DLog(logger, Logger::LogLevel::Warning, errorMessage);
			}
		}
	}
}

#define DX3DGraphicsLogThrowOnFail(hr, message)\
	{\
		auto res = (hr);\
		if (FAILED(res))\
			DX3DLogThrowError(message);\
	}

#define DX3DGraphicsCheckShaderCompile(hr, errorBlob)\
{\
auto res = (hr);\
dx3d::GraphicsLogUtils::CheckShaderCompile(getLogger(), res, errorBlob);\
}