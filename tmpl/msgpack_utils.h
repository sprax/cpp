/// @file: msgpack_utils.cc
/// @auth: sprax
#pragma once

#include <msgpack.hpp>
#include <msgpack/object.hpp>
#include <msgpack/unpack.hpp>
#include <msgpack/type.hpp>
#include <string>
#include <unordered_map>

#include <fstream>
#include <iostream>
#include <sstream>
#include <time.h>


namespace msgpack_utils {

    ///////////////////////////////////////////////////////////////////////////
    ////////////////////// templated msgpack helper functions /////////////////
    template <class T>
    void unpack_deserialize_and_convert(T& odata, const std::string& file_spec, int verbose = 1)
    {
        // Read the file containing the serialized data
        std::ifstream ifs(file_spec, std::ifstream::in);
        std::stringstream buffer;
        buffer << ifs.rdbuf();

        #ifdef ASSERT_TRUE      // only compile this within a testing context
        if (verbose > 2) {
            // Unpack the data from the file straight into a data block and output it
            msgpack::unpacked unpacked_data;
            msgpack::unpack(unpacked_data, buffer.str().data(), buffer.str().size());
            if (verbose > 3) {
                std::cout << "    unpacked: " << unpacked_data.get() << std::endl;
            }
        }
        #endif

        // Unpack the data from the file into a data block and get a handle to it
        msgpack::object_handle oh = msgpack::unpack(buffer.str().data(), buffer.str().size());
        // Get the data block from the handle (unpacked_data =?= deserialized)
        msgpack::object deserialized = oh.get();
        #ifdef ASSERT_TRUE      // only compile this within a testing context
        if (verbose > 1) {
            std::cout << "deserialized: " << deserialized << std::endl;
        }
        #endif
        deserialized.convert(odata);  // void return type
    }


    template <class T>
    bool load_msg_pack(T& odata, const std::string& file_spec, int verbose = 1)
    {
        try {
            unpack_deserialize_and_convert(odata, file_spec, verbose);
            return true;
        }
        catch(const std::exception& ex) {
            // NOTE on exeption types and messages:
            // "insufficient bytes" is thrown when the file_spec is not found
            // std::bad_cast is thrown if odata's type does not match the file's contents
            momap::log()->warn("Error in load_msg_pack({}): {}", file_spec, ex.what());
        }
        return false;
    }


    template <class T>
    bool save_msg_pack(const T& idata, const std::string& file_spec)
    {
        try {
            // Pack the input data and write it to file.
            std::ofstream ofs(file_spec);
            msgpack::pack(ofs, idata);
            return true;
        }   // ofstream destructor closes the file here.
        catch(const std::exception& ex) {
            momap::log()->warn("Error in save_msg_pack({}): {}", file_spec, ex.what());
        }
        return false;
    }

    template <class T>
    time_t test_msg_pack( const T& idata, T& odata
                        , const std::string& file_spec
                        , int max_seconds = 10
                        , int verbose = 1
    ) {
        if (save_msg_pack(idata, file_spec)) {
            // Check that file exists and was created or modified within the last max_seconds seconds.
            time_t file_age_seconds = expect_file_mod(file_spec, max_seconds);

            // Read the file, unpack its data, and convert it to the original input data type.
            unpack_deserialize_and_convert(odata, file_spec, verbose);
            return file_age_seconds;
        }
        return LONG_MAX;
    }

    /// Convert .bin file spec to .mpac file spec:
    ///     If the input name ends in .bin, replace it with .mpac.
    ///     Otherwise, just append .mpac.
    std::string bin_to_mpac_filename(const std::string& bin_file_spec);

}   //  namespace dracula_utils
