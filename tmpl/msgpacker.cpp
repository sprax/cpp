// msgpacker.cpp : template classes using enum values as template parameters
// Requires C11 or later
// BUILD: clang++ -std=c++11 msgpacker.cpp -o pack.out && ./pack.out
#if 1
#include <CLI11.hpp>
#else
#include <CLI/CLI.hpp>
#endif
#include <msgpack.hpp>
#include <limits.h>
#include <iostream>
#include <string>
#define SQRT_2 1.4142135623730



template <typename T>
class NoDefConPair {
public:
    NoDefConPair(std::string nom, T val) : name(nom), value_(val) { }
    NoDefConPair() : name("default_name"), value_(sqrt(2.0)) { }    // default constructor
    T value() { return value_; }
public:
    MSGPACK_DEFINE(name, value_);
    std::string name;
//private:
    T value_;
};

namespace msgpack {
    MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {
        namespace adaptor {
            template <typename T>
            struct as< NoDefConPair<T> > {
                NoDefConPair<T> operator()(msgpack::object const& o) const {
                    if (o.type != msgpack::type::ARRAY)
                        throw msgpack::type_error();
                    if (o.via.array.size != 2)
                        throw msgpack::type_error();
                    return NoDefConPair<T>( o.via.array.ptr[0].as<std::string>()
                                          , o.via.array.ptr[1].as<T>()
                    );
                }
            };
        } // adaptor
    } // MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS)
} // msgpack


namespace std {
    template<typename T>
    string to_string(NoDefConPair<T> ndcp) {
        return "NoDefConPair(name=" + ndcp.name + ", \tvalue=" + std::to_string(ndcp.value()) + ")";
    }
}


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

// hex_dump is not a part of msgpack-c.
inline std::ostream& hex_dump(std::ostream& o, std::string const& i_map) {
    std::ios::fmtflags f(o.flags());
    o << std::hex;
    for (auto c : i_map) {
        o << "0x" << std::setw(2) << std::setfill('0') << (static_cast<int>(c) & 0xff) << ' ';
    }
    o.flags(f);
    return o;
}

template <class K, class V>
int read_into_unordered_map( std::unordered_map<K, V>& dst_map
                           , const std::string& full_path
                           , unsigned max_bytes
                           , int verbose = 1
)  {
    // read from saved file
    void *v_result = malloc(max_bytes);
    if (! read_binary_file(full_path, v_result, max_bytes)) {
        std::cerr << "read_binary_file FAILED!" << std::endl;
        return 2;
    }

    const char *c_result = (const char *)v_result;
    msgpack::object_handle oh = msgpack::unpack(c_result, max_bytes);

    // deserialized object is valid as long as the msgpack::object_handle
    // instance is alive.
    msgpack::object deserialized = oh.get();

    // msgpack::object supports ostream.
    std::cout << deserialized << std::endl;

    // // convert msgpack::object instance into the original type.
    // // if the type is mismatched, it throws msgpack::type_error exception.
    // msgpack::type::tuple<int, bool, std::string> dst;

    deserialized.convert(dst_map);

    std::cout << "Printing deserialized and converted dst_map" << std::endl;
    for (auto const& pr : dst_map) {
        std::cout << pr.first  // string (key)
                  << ":\t"
                  << std::to_string(pr.second) // string's value
                  << std::endl;
    }
    return 0;
}

int test_msgpack_bin(void)
{
    // serializes multiple objects into one message containing a map using msgpack::packer.
    msgpack::sbuffer w_buffer;
    // auto console = spdlog::stdout_color_mt("console");

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

    int verbose = 2;
    std::unordered_map<std::string, double> r_map;
    return read_into_unordered_map(r_map, full_path, max_bytes, verbose);
}


struct Thing {
    char        chr = SCHAR_MAX;
    short       shn = SHRT_MIN;
    int         tin = INT_MAX;
    long        lin = LONG_MIN;
    long long   lln = LLONG_MAX;
    MSGPACK_DEFINE(chr, shn, tin, lin, lln);
};


int test_thing()
{
    Thing thing;

    std::string path("thing.bin");
    {
        std::ofstream ofs(path);
        msgpack::pack(ofs, thing);
    }   // ofstream destructor closes file here.

    // Deserialize the serialized data
    std::ifstream ifs(path, std::ifstream::in);
    std::stringstream buffer;
    buffer << ifs.rdbuf();
    msgpack::unpacked upd;
    msgpack::unpack(upd, buffer.str().data(), buffer.str().size());
    std::cout << upd.get() << std::endl;
    return 0;
}


int test_msgpack_unordered_map()
{
    std::stringstream ss;
    std::unordered_map<std::string, NoDefConPair<double> > r_map;
    std::unordered_map<std::string, NoDefConPair<double> > i_map { { "ABC", {"pi", 3.14159 }}
                                                               , { "DEFG", {"e", 2.71828 }} };
    msgpack::pack(ss, i_map);
    hex_dump(std::cout, ss.str()) << std::endl;

    unsigned max_bytes = ss.str().size();
    // write to file
    void *data = (void *)ss.str().data();
    std::string full_path = "packed.map";
    if (! write_binary_file(full_path, data, max_bytes)) {
        std::cerr << "write_binary_file FAILED!" << std::endl;
        return 1;
    }

    // read from saved file
    int verbose = 2;

#if 1
    return read_into_unordered_map(r_map, full_path, max_bytes, verbose);
#else   // FIXME BUSTED: return read_into_unordered_map(r_map, full_path, max_bytes, verbose);
    return -1;
#endif
}

#define DEFINE_DEFAULT_CONSTRUCTOR 0

struct NoDefConDblFlt {
    NoDefConDblFlt(double x, float y) : x(x), y(y)  { }

#if DEFINE_DEFAULT_CONSTRUCTOR
#error "Don't do it!  DEFINE_DEFAULT_CONSTRUCTOR"
    NoDefConDblFlt() : x(1.0/SQRT_2), y(SQRT_2)  { }
#else
    NoDefConDblFlt() = delete;
#endif

    double x;
    float y;
    MSGPACK_DEFINE(x, y);
};

/// Define 'as' class template specialization as follows:
namespace msgpack {
    MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {
        namespace adaptor {
            template <>
            struct as<NoDefConDblFlt> {
                NoDefConDblFlt operator()(msgpack::object const& o) const {
                    if (o.type != msgpack::type::ARRAY)
                        throw msgpack::type_error();
                    if (o.via.array.size != 2)
                        throw msgpack::type_error();
                    return NoDefConDblFlt( o.via.array.ptr[0].as<double>()    // type does not seem to
                                         , o.via.array.ptr[1].as<float>());   // matter, not even size
                }
            };
        } // adaptor
    } // MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS)
} // msgpack


int test_no_def_con()
{
    NoDefConDblFlt i_ndcdf(23.232323, SQRT_2);
    std::string path("i_ndcdf.bin");
    {
        std::ofstream ofs(path);
        msgpack::pack(ofs, i_ndcdf);
    }   // ofstream destructor closes file here.

    // Deserialize the serialized data
    std::ifstream ifs(path, std::ifstream::in);
    std::stringstream buffer;
    buffer << ifs.rdbuf();
    msgpack::unpacked upd;
    msgpack::unpack(upd, buffer.str().data(), buffer.str().size());

    msgpack::object r_ndcdf_object = upd.get();
    std::cout << "Got unpacked MsgPack object: r_ndcdf_object: " << r_ndcdf_object << std::endl;

    NoDefConDblFlt r_ndcdf = r_ndcdf_object.as<NoDefConDblFlt>();
    std::cout << "Got newly constructed NoDefConDblFlt: " << r_ndcdf.x << " | " << r_ndcdf.y << std::endl;

    ///////////////////////////////////////////////////////////////////////////
    // conclusion from below: The msgpack adaptor can deserialize an
    // object that has no default constructor, but std::map (and unordered_map)
    // still do need a default constructor for their value types.
#if DEFINE_DEFAULT_CONSTRUCTOR
    std::map<std::string, NoDefConDblFlt> r_map, i_map;
    std::stringstream ss;
    msgpack::pack(ss, i_map);
    hex_dump(std::cout, ss.str()) << std::endl;
    unsigned max_bytes = ss.str().size();
    // write to file
    void *data = (void *)ss.str().data();
    std::string full_path = "packed_ndcdf_map.bin";
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
    msgpack::object deserialized = oh.get();
    std::cout << deserialized << std::endl;
    deserialized.convert(r_map);
    // show the converted map...
#endif
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
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

    NoDefConPair<double> ndcp("hello", 0.987654321);
    std::cout << "ndcp: " << std::to_string(ndcp) << std::endl;

    std::cout << MSGPACK_VERSION << std::endl;
    int bad = test_msgpack_bin();
    int err = test_msgpack_unordered_map();
    int res = test_thing();
    int eno = test_no_def_con();
    return 0;
}
