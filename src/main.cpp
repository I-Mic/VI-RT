//
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#include <exception>
#include <iostream>
#include <memory>

#include "config/config_parser.hpp"
#include "image/image.hpp"

int main(int const argc, char const* const* const argv){

    if(argc == 1){
        std::cerr << "usage: " << argv[0] << " CONFIG_FILE...\n";
        return 1;
    }

    int status {0};

    for(int i {1}; i < argc; ++i){

        try {
            std::unique_ptr<config_parser_t> const config_obj {
                config_parser_t::from_toml(argv[i])
            };

            std::unique_ptr<image_t> const img {config_obj->build_image()};
            if(!img->output_image()){
                std::cerr << "Error writing output\n";
                status = 2;
            }
        }
        catch(std::exception const& e){
            std::cerr << argv[0] << ": " << e.what() << '\n';
            status = 3;
        }
   }

    return status;
}
