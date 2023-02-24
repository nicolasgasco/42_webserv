#pragma once

class Config
{
	public:
		Config();
		~Config();
    
		void	process_config_file(char* config_file);
};
