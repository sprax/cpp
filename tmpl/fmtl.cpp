// fmtl.cpp : template classes using enum values as template parameters
// Requires C11 or later.
/** Example  build commands:
    g++ -std=c++14 fmtl.cpp -I/opt/drake/include/eigen3 -I ~/asdf/spdlog/include -o tmpl.out && tmpl.out
clang++ -std=c++14 fmtl.cpp -I/opt/drake/include/eigen3 -I ~/asdf/spdlog/include -o tmpl.out && tmpl.out
******************/
#include "../../spdlog/include/spdlog/spdlog.h"

#include <iostream>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <Eigen/Dense>

using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::fixed;
using std::setw;
using std::showpos;
using std::string;
using std::to_string;
using std::vector;
using std::lround;

#if 1
#define INT_ROUND(_a) std::lround(_a)
#else
#define INT_ROUND(_a) int(_a)   // truncate
#endif

typedef char chr;

class myclass
{
friend std::ostream& operator<<(std::ostream&, myclass const&);

public:
    myclass(int i) : k_(i) { }

private:
    int k_;
    const double sqrt2 = 1.41;
};

std::ostream &operator<<(std::ostream& os, myclass const& m) {
    return os << m.k_ << " sqrt2: " << m.sqrt2;
}

std::string endpath_after_dir(const std::string& path, const std::string& dir)
{
    size_t beg = 0, pos = path.find(dir, 1);
    if (pos == string::npos) {
        return "";
    }
    beg = 1 + pos + dir.size();
    std::string endpath = path.substr(beg);
    cout << "find " << dir << " in " << path << " gives pos: " << pos << " so beg = " << beg << endl;
    cout <<  "Thus the endpath is: (" << endpath << ")" << endl;
    return endpath;
}



/// Round any number to an int
template <typename NUM>
inline static int IntRound(NUM num) {
    return static_cast<int>(std::lround(num));
}

/// Multiply input value by factor, round the result to an integral value,
/// and return that separated into an unsigned int and a '+' or '-' character
/// corresponding to the result's sign, where the sign of 0 is always plus.
int scaled_uint_and_sign( uint& uint_result, char& sign_result
                        , const double value, const double factor
                        , const uint max_result, const std::string id_name
) {
    int sint_result = IntRound(factor * value);
    if (sint_result < 0.0) {
        sign_result = '-';
        uint_result = -sint_result;
    } else {
        sign_result = '+';
        uint_result = sint_result;
    }
    if (uint_result > max_result) {
        string err_msg = fmt::format("bad ID({}) result {} > {} max"
                                    , id_name, uint_result, max_result
        );
        ////assert(uint_result <= max_result && err_msg.c_str());
        throw std::runtime_error(err_msg);
        return 1;
    }
    return 0;
}


void show_paths()
{
    string path("/homes/s2/usr/sprax/traj_lib/truffle/digger/get_it");
    string lib_dir("traj_lib");
    size_t beg = 0, pos = path.find(lib_dir, 1);
    if (pos != string::npos) {
        beg = 1 + pos + lib_dir.size();
        string subpath = path.substr(beg);
        cout << "find " << lib_dir << " in " << path << " gives pos: " << pos << " so beg = " << beg << endl;
        cout <<  "Thus the subpath is: (" << subpath << ")" << endl;
    }
    string endpath = endpath_after_dir(path, lib_dir);
    cout << "FINALLY endpath: (" << endpath << ")" << endl;
}


/// Returns a key position ID string formatted as _x=XXXX_y=YYYY_z=ZZZZ, where each
/// 3D vector component is scaled and rounded into the integer range [-999, 999]
/// and converted to a 4-character string in the ASCII string range ["-999", "0999"]
/// Examples:
/// (-1.0, -0.6660, 1.0) => "_x=-999_y=-665_z=0999"
/// (-1.0, -0.6666, 1.0) => "_x=-999_y=-666_z=0999"
/// A runtime_error is thrown if the output from the vector would be out of range;
/// basically, if any input vector component is outside the range [-1.0, 1.0].
/// If you want all the digits to be maximally significant (dispersion), prescale the input vectors.
/// If you prefer that the numbers in the names approximate millimeters, make sure the input vectors
/// are measured in meters.
const std::string FractionIdKeyXyz(const Eigen::Vector3d& vec3d)
{
    static const std::string sep("_");
    static const double scale = 999.0;
    static const uint key_max = 1000;
    static const double max_cam = (scale + 0.4444) / scale; // ensure lround(max_cam * scale) == 999
    double cam = vec3d.cwiseAbs().maxCoeff();
    if (cam > max_cam) {
        string err_msg = fmt::format("bad ID key: {}", cam);
        throw std::runtime_error(err_msg);
    }
    uint x_uint, y_uint, z_uint;
    char x_sign, y_sign, z_sign;
    try {
        scaled_uint_and_sign(x_uint, x_sign, vec3d[0], scale, key_max, "X");
        scaled_uint_and_sign(y_uint, y_sign, vec3d[1], scale, key_max, "Y");
        scaled_uint_and_sign(z_uint, z_sign, vec3d[2], scale, key_max, "Z");
    } catch(const std::exception& ex) {
        cerr << "FractionIdKeyXyz: Error: " << ex.what() << endl;
        throw;
    }
    std::ostringstream oss;
    oss.precision(0);
    oss.fill('0');
    oss << sep << "x=" << x_sign << setw(3) << x_uint
        << sep << "y=" << y_sign << setw(3) << y_uint
        << sep << "z=" << z_sign << setw(3) << z_uint;
    return oss.str();
}

/// Returns an angle-pair ID string formatted as _theta=TTTT_phi=PPPP,
/// where the angles theta and phi, input in radians, are scaled and rounded
/// into the integer range [-900, 900], so the unit is a fifth of a degree (FOD).
/// So a 30 degree angle is 150 FODs, 90 => 450, etc.
/// Each of these is converted to a 4-character string in the ASCII string range ["-900", "0900"]
/// Examples:
/// (-3.14159265359, 3.14159265359) => "_=theta=-900_phi=900"
/// (-1.0, 1.0) =>  => "_=theta=-286_phi=0286"
/// A runtime_error is thrown if the output would be out of range;
/// basically, if either input angle is outside the range [-M_PI, M_PI].
/// NOTE: Plus or minus PI may be viewed as the same singular angle, to be avoided.
/// So there is no round-off leniency or "fudge factor" here; any angle close to 180 degrees
/// is already suspect.
const std::string FractionIdThetaPhi(double theta, double phi)
{
    static const std::string sep("_");
    static const double fod_factor = 900.0 / M_PI;  // factor to convert radians to FODs
////static const int fod_min = -901;
    static const int fod_max = 901;

    char theta_sign, phi_sign;
    uint theta_fods, phi_fods;
    try {
        scaled_uint_and_sign(theta_fods, theta_sign, theta, fod_factor, fod_max, "theta");
        scaled_uint_and_sign(phi_fods, phi_sign, phi, fod_factor, fod_max, "phi");
    } catch(const std::exception& ex) {
        cerr << "FractionIdThetaPhi: Error: " << ex.what() << endl;
        throw;
    }

    std::ostringstream oss;
    oss.precision(0);
    oss.fill('0');
    oss << sep << "theta="  << theta_sign << setw(3) << theta_fods
        << sep << "phi="    << phi_sign   << setw(3) << phi_fods;
    return oss.str();
}


void show_key_ids()
{
    double key_min = -1.0, key_max = 1.0;
    for (double xx = key_min; xx <= key_max; xx += 1.0) {
        for (double yy = key_min; yy <= key_max; yy += 0.5) {
            for (double zz = key_min; zz <= key_max; zz += 0.666666) {
                Eigen::Vector3d vec3d(xx, yy, zz);
                const std::string key_part = FractionIdKeyXyz(vec3d);
                cerr<< "key_part: " << key_part << "\t from:"
                    << "\t" << setw(9) << xx
                    << "\t" << setw(9) << yy
                    << "\t" << setw(9) << zz
                    << endl;
            }
        }
    }
    double max_cam = 999.4444 / 999.0;
    string mystr = fmt::format("hahaha {} < {}", 300*M_PI, lround(max_cam*999));
    cerr << mystr << endl;
}


void show_angle_ids()
{
    double rad_min = -M_PI, rad_max = M_PI;
    for (double theta = rad_min; theta <= rad_max; theta += 1.0) {
        for (double phi = rad_min; phi <= rad_max; phi += 0.4) {
            const std::string angle_part = FractionIdThetaPhi(theta, phi);
            cerr<< "angle_part: " << angle_part << "\t from:"
                << "\t" << setw(9) << theta
                << "\t" << setw(9) << phi
                << endl;
        }
    }
}

void show_key_angle_ids()
{
    double key_min = -1.0, key_max = 1.0;
    double rad_min = -M_PI, rad_max = M_PI;
    for (double xx = key_min; xx <= key_max; xx += 1.0) {
        for (double yy = key_min; yy <= key_max; yy += 0.5) {
            for (double zz = key_min; zz <= key_max; zz += 0.666666) {
                Eigen::Vector3d vec3d(xx, yy, zz);
                const std::string key_part = FractionIdKeyXyz(vec3d);
                for (double theta = rad_min; theta <= rad_max; theta += M_PI/3.0) {
                    for (double phi = rad_min; phi <= rad_max; phi += M_PI/4.0) {
                        const std::string angle_part = FractionIdThetaPhi(theta, phi);
                        cerr<< "ID: " << key_part << angle_part << "\t from:"
                            << "\t" << setw(9) << xx
                            << "\t" << setw(9) << yy
                            << "\t" << setw(9) << zz
                            << "\t" << setw(9) << theta
                            << "\t" << setw(9) << phi
                            << endl;
                    }
                }
            }
        }
    }
}

/***********************************************************************
ONE_PLUS_EPS = 1.0 + sys.float_info.epsilon
def num_digits_log(num, base=10):
    '''returns number of digits in num as a non-negative decimal integer.
    Unless base != 10, in which case it returns the number of digits num
    would have in the specified base representation.
    NOTE: Without the ONE_PLUS_EPS factor, num_digits_log(1000 would return 3, not 4.
    '''
    return 1 + int(math.log(num * ONE_PLUS_EPS, base))
***********************************************************************/


/// returns number of digits in num as a non-negative decimal integer.
/// NOTE: Without the ONE_PLUS_EPS factor, num_digits_log(1000 would return 3, not 4.
unsigned num_digits_log(double num)
{
    // static const double ONE_PLUS_EPS = 1.0 + std::numeric_limits<double>::epsilon();
    // return 1 + unsigned(std::log10(num * ONE_PLUS_EPS));
    return 1 + unsigned(std::log10(num));
}

void show_num_digits() {
    double num = 0.0;
    cerr << "num_digits_log(" << num << ") : \t" << num_digits_log(num) << endl;
    num = 9.9;
    cerr << "num_digits_log(" << num << ") : \t" << num_digits_log(num) << endl;
    num = 10.0;
    cerr << "num_digits_log(" << num << ") : \t" << num_digits_log(num) << endl;
    num = 99.9999999;
    cerr << "num_digits_log(" << num << ") : \t" << num_digits_log(num) << endl;
    num = 100;
    cerr << "num_digits_log(" << num << ") : \t" << num_digits_log(num) << endl;
    num = 1000;
    cerr << "num_digits_log(" << num << ") : \t" << num_digits_log(num) << endl;
    num = 10000;
    cerr << "num_digits_log(" << num << ") : \t" << num_digits_log(num) << endl;
    num = 100000;
    cerr << "num_digits_log(" << num << ") : \t" << num_digits_log(num) << endl;
    num = 999999.9999999;
    cerr << "num_digits_log(" << num << ") : \t" << num_digits_log(num) << endl;
    num = 1000000.0;
    cerr << "num_digits_log(" << num << ") : \t" << num_digits_log(num) << endl;
    num = 10000000.0;
    cerr << "num_digits_log(" << num << ") : \t" << num_digits_log(num) << endl;
}


/// Converts an angle in radians to an unsigned int and a sign character,
/// where the unsigned int is proportional to the smallest equivalent of
/// the orginal angle as measured counterclockwise (positive orientation)
/// or clockwise (negative orientation), and all angles are equivalent
/// modulo two PI.  In other words, the usigned result represents some
/// number of fractions of a degree, and the sign result indicates whether
/// the number is measured counterclockwise (+) or clockwise (-).
/// Examples with the default units of Pi radians = 180 degrees,
/// and thus the valid interval for unsigned result is exactly [0, 180]:
///  1*Pi/6 => ( 30, '+')
///  2*Pi/3 => (120, '+')
///  4*Pi/3 => (120, '-')    Not 240, which is not in [0, 180]
///  7*Pi/6 => (150, '-')    Not 210
///  7*Pi/3 => ( 60, '+')    Not 420, nor -300
/// -7*Pi/3 => ( 60, '-')    Not 300, nor -420
/// -1*Pi/3 => ( 60, '-')    Not 300
/// -5*Pi/4 => (135, '+')    Not -225
/// NOTE: Zero is never negative, that is, the sign of a 0 result is always '+'.
int angle_uint_and_sign( uint& uint_result, char& sign_result
                       , const double angle
                       , const double degree_factor = 180.0
                       , const std::string id_name = ""
) {
    double dbl_multiple;
    double fraction = std::modf(angle/M_PI, &dbl_multiple);
    int    int_multiple = std::lround(dbl_multiple);

    bool opp_sign = false;
    if (fraction < 0.0) {
        fraction = -fraction;
        opp_sign = true;
    }

    if (int_multiple % 2) {
        uint_result = IntRound(degree_factor * (1.0 - fraction));
        sign_result = opp_sign ? '+' : '-';
    } else {
        uint_result = IntRound(degree_factor * fraction);
        sign_result = opp_sign ? '-' : '+';
    }

    cerr<< "angle_uint_and_sign(angle=" << angle << "=" << angle/M_PI << "p=" << 180*angle/M_PI
        << "d, deg_fac=" << degree_factor << ") => "
        << "fraction: " << fraction << ", int_mult: " << int_multiple
        << ", uint_res: " << uint_result << ", sign_result: " << sign_result
        << " ==> '" << sign_result << uint_result << "'"
        << endl;
    return 0;
}

void show_modfs()
{
#ifdef DONT_DO_IT
    double oome = 1.0/M_E;
    double oor2 = 1.0/sqrt(2.0);
    double eopi = M_E / M_PI;
    double thrd = M_PI / 6.0;

    double fods = 900 / M_PI;

    cerr << "\t1/M_E: " << oome << "\t 1/sqrt2: " << oor2 << " \t M_E/M_PI: " << eopi << " \t pi/6: " << thrd << endl;
    cerr << "\t1/M_E: " << oome*fods << "\t 1/sqrt2: " << oor2*fods << " \t M_E/M_PI: " << eopi*fods << " \t pi/6: " << thrd*fods << endl;

    double angs[]{oome, oor2, eopi, thrd, fods};
    for (auto aa : angs) {
        cerr << "\t " << aa;
    }
    cerr << endl;
    int j = 0;
    for (auto& aa : angs) {
        ++j;
        aa += j * M_PI;
        cerr << "\t " << aa;
    }
    cerr << endl;
    for (auto& aa : angs) {
        aa *= fods;
        cerr << "\t " << aa;
    }
    cerr << endl;
    for (auto& aa : angs) {
        double mult = 0;
        double frac = std::modf(aa, &mult);
        int mint = std::lround(mult);
        cerr << "\t (" << frac << " = modf(" << aa << ", " << mult << ") => " << mint;
    }
    cerr << endl;
    for (auto& aa : angs) {
        double frac = std::fmod(aa, M_PI*2);
        cerr << "\t (" << frac << " = fmod(" << aa << ", " << M_PI*2 << ")  ";
    }
    cerr << endl;
#endif
    int    imult = 0, idegs;
    double dmult = 0.0, frac;
    double twopi =  M_PI * 2.0;

    double dp030 =  M_PI/6.0;
    double dp045 =  M_PI/4.0;
    double dp120 =  M_PI * 2.0 / 3;
    double dp210 =  M_PI * 7.0 / 6.0;
    double dp390 =  M_PI * 13.0 / 6.0;

    double dn030 = -M_PI/6.0;
    double dn045 = -M_PI/4.0;
    double dn120 = -M_PI * 2.0 / 3;
    double dn210 = -M_PI * 7.0 / 6.0;
    double dn390 = -M_PI * 13.0 / 6.0;
    double phi315 = 5.497787143782138;
    double angles[]{dp030, dp045, dp120, dp210, dp390, dn030, dn045, dn120, dn210, dn390, phi315, twopi};

    frac = std::modf(dp030/M_PI, &dmult);
    imult = std::lround(dmult);
    idegs = std::lround(imult % 2 ? -180.0 * frac : 180.0 * frac);
    cerr << " pi/6 -> modf.frac: " << frac << ", modf.imult: " << imult << ", idegs: " << idegs << endl;


    frac = std::modf(dp390/M_PI, &dmult);
    imult = std::lround(dmult);
    idegs = std::lround(imult % 2 ? -180.0 * frac : 180.0 * frac);
    cerr << " pi*7/6 => " << idegs << endl;


    int err;
    uint uint_result;
    char sign_result;
    double degree_factor = 180.0;       // use 900.0 for units of one fifth of a degree
    const std::string id_name = "Angle";

    for (double angle : angles) {
        err = angle_uint_and_sign(uint_result, sign_result, angle, degree_factor, id_name);
        cerr << "angle_uint_and_sign -> " << err << " (" << uint_result << ", " << sign_result << ")" << endl;
    }
}

void show_ptr_inc()
{
    auto a = std::make_shared<int>(42);
    auto b = a;
    *a = 43;
    cout << "*b after *a = 43 is now: " << *b << endl;
    (*a.get())++;
    cout << "*b after (*a.get())++ is now: " << *b << endl;
    ++*a;
    cout << "*b after ++*a is now: " << *b << endl;
}

int main() {
    myclass xx(10);
    myclass *my_ptr = NULL;

    std::cout << "myclass: " << xx << std::endl;
    time_t tt = time(nullptr);
    std::cout << "my time: " << tt << std::endl;
    delete my_ptr;
    double pi;
    if ((pi = M_PI) && (pi > 3)) {
        cout << "M_PI == " << pi << " is > 3" << endl;
    }

    // assert(false && "asserted false");
    // show_ptr_inc();
    // show_paths();
    // show_angle_ids();
    // show_key_ids();
    // show_key_angle_ids();
    // show_num_digits();
    show_modfs();
    return 0;
}
