// msgpacker.cpp : template classes using enum values as template parameters
// Requires C11 or later
// BUILD: clang++ -std=c++11 msgpacker.cpp -o pack.out && ./pack.out
#if 1
#include <CLI11.hpp>
#else
#include <CLI/CLI.hpp>
#endif
#include <msgpack.hpp>

#include <iostream>
#include <string>


bool write_binary_file(const std::string& full_path, void *data, unsigned max_bytes)
{
    try {
        std::ofstream ofs(full_path, std::fstream::binary|std::fstream::out);
        ofs.write((char *)data, max_bytes);
        ofs.close();                            // TODO: needed?
        return true;
    } catch (const std::exception& ex) {
        std::cerr << "Error in write_binary_file: " << ex.what() << std::endl;
    }
    return false;
}

bool read_binary_file(const std::string& full_path, void *result, unsigned max_bytes)
{
    try {   // Don't check specifics, just catch any error.
        std::ifstream ifs(full_path, std::fstream::binary|std::fstream::in);
        if (! ifs.good()) {
            std::cerr << "Failed to open file  for binary read: " << full_path << std::endl;
            return false;
        }
        if (!ifs.read((char *)result, max_bytes)) {
            std::cerr << "Error reading binary file: " << full_path << std::endl;
            return false;
        }
        return true;
    } catch (const std::exception& ex) {
        std::cerr << "Error in read_binary_file: " << ex.what() << std::endl;
    }
    return false;
}



int test_msgpack(void)
{
    // serializes multiple objects into one message containing a map using msgpack::packer.
    msgpack::sbuffer w_buffer;

    msgpack::packer<msgpack::sbuffer> packer(&w_buffer);
    packer.pack_map(2);
    packer.pack(std::string("x"));
    packer.pack(4);
    packer.pack(std::string("y"));
    packer.pack(3.4321);

    unsigned max_bytes = w_buffer.size();
    // write to file
    void *data = w_buffer.data();
    std::string full_path = "packed.bin";
    if (! write_binary_file(full_path, data, max_bytes)) {
        std::cerr << "write_binary_file FAILED!" << std::endl;
        return 1;
    }

    // read from saved file
    void *v_result = malloc(max_bytes);
    if (! read_binary_file(full_path, v_result, max_bytes)) {
        std::cerr << "read_binary_file FAILED!" << std::endl;
        return 2;
    }

    const char *c_result = (const char *)v_result;
    msgpack::object_handle oh = msgpack::unpack(c_result, max_bytes);

     // deserialized object is valid during the msgpack::object_handle instance is alive.
     msgpack::object deserialized = oh.get();

     // msgpack::object supports ostream.
     std::cout << deserialized << std::endl;

     // // convert msgpack::object instance into the original type.
     // // if the type is mismatched, it throws msgpack::type_error exception.
     // msgpack::type::tuple<int, bool, std::string> dst;

    std::map<std::string, float> dst_map;
    deserialized.convert(dst_map);

    std::cout << "Printing deserialized and converted dst_map" << std::endl;
    for (auto const& pr : dst_map) {
        std::cout << pr.first  // string (key)
                  << ":\t"
                  << pr.second // string's value
                  << std::endl;
    }
    return 0;
}

int main(int argc, char **argv) {
    CLI::App app{"App description"};

    // Define options
    bool f;
    app.add_flag("-f", f, "flag f");

    int p = 0;
    app.add_option("-p", p, "int parameter");

    std::string s = "s";
    app.add_option("-s,--str", s, "string parameter");

    CLI11_PARSE(app, argc, argv);

    std::cout << "bool flag value: " << f << std::endl;
    std::cout << "int param value: " << p << std::endl;
    std::cout << "str param value: " << s << std::endl;
    std::cout << "===========================" << std::endl;
    std::cout << "Examples of bad command(s):" << std::endl;
    std::cout << argv[0] << " -str=my_str" << std::endl;

    std::string ts("first part, " "second part.");
    std::cout << "TEST STRING: " <<  ts << std::endl;

    s = "";
    if (s.empty()) {
        std::cout << "nothing is true, and truth is empty!  (" << s << ")" << std::endl;
    } else {
        std::cout << "nothing is not only empty, it's also false!  (" << s << ")" << std::endl;
    }

    std::cout << MSGPACK_VERSION << std::endl;
    return test_msgpack();
}
