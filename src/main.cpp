//
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#include <iostream>

#include "config/config_parser.hpp"
#include "image/image.hpp"


int main(int const argc, char const* const* const argv){

    if(argc == 1){
        std::cerr << "usage: " << argv[0] << " CONFIG_FILE...\n";
        return 1;
    }

    for(int i = 1; i < argc; ++i){

        std::unique_ptr<config::config_parser_t> const config_obj {
            config::config_parser_t::from_toml(argv[i])
        };

        std::unique_ptr<img::image_t> img {config_obj->build_image()};
        if(!img->output_image()){
            std::cerr << "Error writing output\n";
            return 2;
        }
    }

    return 0;
}
