#include <vepch.h>
#include "Log.h"

// Headers
#include <Config.h>

#include <string>
#include <vector>
#include <memory>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace Voxel
{

	std::shared_ptr<spdlog::logger> Log::logger;

	void Log::Init(const char* filename)
	{
		std::vector<spdlog::sink_ptr> sinks;
		sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		sinks.at(0)->set_level(spdlog::level::trace);

		sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(filename, true));
		sinks.at(1)->set_level(spdlog::level::trace);

		logger = std::make_shared<spdlog::logger>("VoxelEngine Logger", begin(sinks), end(sinks));
		logger->set_pattern("%^[%X] %l: %v%$");
	}

} // namepsace 