#pragma once

// Headers
#include <Config.h>
#include <memory>
#include <spdlog/spdlog.h>

namespace Voxel
{

	class Log
	{
	public:
		static void Init(const char* filename);
		inline static std::shared_ptr<spdlog::logger>& GetLogger() { return _logger; }
	private:
		static std::shared_ptr<spdlog::logger> _logger;
	};

// Log macros
#define VE_LOG_TRACE(...)		Voxel::Log::GetLogger()->trace(__VA_ARGS__)
#define VE_LOG_DEBUG(...)		Voxel::Log::GetLogger()->debug(__VA_ARGS__)
#define VE_LOG_INFO(...)		Voxel::Log::GetLogger()->info(__VA_ARGS__)
#define VE_LOG_WARN(...)		Voxel::Log::GetLogger()->warn(__VA_ARGS__)
#define VE_LOG_ERROR(...)		Voxel::Log::GetLogger()->error(__VA_ARGS__)
#define VE_LOG_CRITICAL(...)	Voxel::Log::GetLogger()->critical(__VA_ARGS__)

} // namespace Voxel