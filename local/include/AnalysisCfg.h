// -*- c++ -*-
#ifndef TOWERJETS_ANALYSISCFG_H
#define TOWERJETS_ANALYSISCFG_H

#include <string>
#include <cmath>

///@brief Static histogram parameters
namespace Analysis {
  ///@brief Units names and values
  namespace Units { 
    ///@brief Unit value
    ///
    /// The value of a unit is defined with respect to the usual convention @f$ h = c = 1 @f$, if possible. 
    /// This is basically a copy of CLHEP (not maintained anymore), with some minor modifications.
    ///    namespace Value { 
    ///@name Energy units
    ///@{
    static const double MeV = 1.0;        ///<@brief Megaelectronvolt
    static const double keV = MeV/1000.;  ///<@brief Kiloelectronvolt
    static const double eV  = keV/1000.;  ///<@brief ElectronVolt
    static const double GeV = MeV*1000.;  ///<@brief Gigaelectronvolt
    static const double TeV = GeV*1000.;  ///<@brief Terraelectronvolt
    ///@}
    ///@name Length
    ///@{
    static const double mm  = 1.0;        ///<@brief Millimeter
    static const double mu  = mm/1000.;   ///<@brief Micrometer
    static const double nm  = mu/1000.;   ///<@brief Nanometer
    static const double pm  = nm/1000.;   ///<@brief Picometer
    static const double fm  = pm/1000.;   ///<@brief Femtometer 
    static const double cm  = mm*10.;     ///<@brief Centimeter
    static const double m   = cm*100.;    ///<@brief Meter
    static const double km  = m*1000.;    ///<@brief Kilometer
    static const double in  = mm*25.4;    ///<@brief Inch
    static const double ft  = in*12.;     ///<@brief Foot
    static const double yd  = in*36.;     ///<@brief Yard
    static const double mi  = ft*5280.;   ///<@brief Mile
    static const double F   = fm;         ///<@brief Fermi
    static const double Ang = nm*10.;     ///<@brief Angstroem
    ///@}
    ///@name Time
    ///@{
    static const double nanosec  = 1.;               ///<@brief Nanosecond
    static const double microsec = nanosec*1000.;    ///<@brief Microsecond
    static const double millisec = microsec*1000.;   ///<@brief Millisecond
    static const double sec      = millisec*1000.;   ///<@brief Second
    static const double min      = sec*60.;          ///<@brief Minute
    static const double hr       = min*60.;          ///<@brief Hour
    static const double day      = hr*24.;           ///<@brief Day
    static const double wk       = day*7.;           ///<@brief Week
    static const double yr       = day*365.;         ///<@brief Year
    static const double mo       = yr/12.;           ///<@brief Month (average for 12 months/year)
    static const double picosec  = nanosec/1000.;    ///<@brief Picosecond
    static const double femtosec = picosec/1000.;    ///<@brief Femtosecond
    static const double attosec  = femtosec/1000.;   ///<@brief Attosecond
    ///@}
    ///@name Areas
    ///@{
    static const double sqmm      = mm*mm;           ///<@brief Squaremillimeter 
    static const double sqcm      = cm*cm;           ///<@brief Squarecentimeter
    static const double sqm       = m*m;             ///<@brief Squaremeter
    static const double sqkm      = km*km;           ///<@brief Squarekilometer
    static const double sqin      = in*in;           ///<@brief Squareinch
    static const double sqft      = ft*ft;           ///<@brief Squarefoot
    static const double sqmi      = mi*mi;           ///<@brief Squaremile
    static const double b         = sqmm*1.0e-22;    ///<@brief Barn
    static const double millib    = b/1000.;         ///<@brief Millibarn
    static const double microb    = millib/1000.;    ///<@brief Microbarn
    static const double nanob     = microb/1000.;    ///<@brief Nanobarn
    static const double picob     = nanob/1000.;     ///<@brief Picabarn
    static const double femtob    = picob/1000.;     ///<@brief Femtobarn
    static const double attob     = femtob/1000.;    ///<@brief Attobarn
    ///@}
    ///@name Charge
    ///@{
    static const double eplus      = 1.0;                 ///<@brief Positron charge
    static const double eplus_si   = 1.602176487e-19;     ///<@brief Positron charge in SI units
    static const double e          = eplus;               ///<@brief Positron charge
    static const double Coulomb    = eplus/eplus_si;      ///<@brief Coulomb
    static const double C          = Coulomb;             ///<@brief Coulomb
    ///@}
    ///@name Force, energy, power, mass, pressure in SI units
    ///@{ 
    static const double Joule       = eV/eplus_si;         ///<@brief Joule
    static const double J           = Joule;               ///<@brief Joule
    static const double Watt        = Joule/m;             ///<@brief Watt
    static const double W           = Watt;                ///<@brief Watt
    static const double kiloWatt    = Watt*1000.;          ///<@brief Kilowatt
    static const double kW          = kiloWatt;            ///<@brief Kilowatt
    static const double MegaWatt    = kiloWatt*1000.;      ///<@brief Megawatt
    static const double MW          = MegaWatt;            ///<@brief Megawatt
    static const double Newton      = Joule/m;             ///<@brief Newton
    static const double N           = Newton;              ///<@brief Newton
    static const double kg          = Joule*sec*sec/(m*m); ///<@brief Kilogram
    static const double g           = kg/1000.;            ///<@brief Gram
    static const double millig      = g/1000.;             ///<@brief Milligram
    static const double microg      = millig/1000.;        ///<@brief Microgram
    static const double Pascal      = Newton/sqm;          ///<@brief Pascal
    static const double P           = Pascal;              ///<@brief Pascal
    static const double hectoPascal = Pascal*100.;         ///<@brief Hectopascal
    static const double hectoP      = hectoPascal;         ///<@brief Hectopascal
    static const double bar         = Pascal*100000.;      ///<@brief Bar
    static const double millibar    = bar/1000.;           ///<@brief Millibar
    static const double atmo        = Pascal*101325.;      ///<@brief Atmosphere
    ///@}
    ///@name Electrical units
    ///@{
    static const double Ampere      = Coulomb/sec;        ///<@brief Ampere (current)
    static const double A           = Ampere;             ///<@brief Ampere (current)
    static const double milliA      = A/1000.;            ///<@brief Milliampere (current)
    static const double microA      = milliA/1000.;       ///<@brief Microampere (current)
    static const double nanoA       = microA/1000.;       ///<@brief Nanoampere (current)
    static const double picoA       = nanoA/1000.;        ///<@brief Picoampere (current)
    static const double Volt        = eV/eplus;           ///<@brief Volt (potential)
    static const double V           = Volt;               ///<@brief Volt (potential)
    static const double milliV      = V/1000.;            ///<@brief Millivolt (potential)
    static const double kV          = V*1000.;            ///<@brief Kilovolt (potential)
    static const double MV          = kV*1000.;           ///<@brief Megavolt (potential)
    static const double Ohm         = V/A;                ///<@brief Ohm (resistance)
    static const double kiloOhm     = Ohm*1000.;          ///<@brief Kiloohm (resistance)
    static const double MegaOhm     = kiloOhm*1000.;      ///<@brief Megaohm (resistance)
    static const double Farad       = Coulomb/Volt;       ///<@brief Farad (capacitance)
    static const double milliFarad  = Farad/1000.;        ///<@brief Millifarad (capacitance)
    static const double microFarad  = milliFarad/1000.;   ///<@brief Microfarad (capacitance)
    static const double nanoFarad   = microFarad/1000.;   ///<@brief Nanofarad (capacitance)
    static const double picoFarad   = nanoFarad/1000.;    ///<@brief Picofarad (capacitance)
    static const double Weber       = Volt*sec;           ///<@brief Weber (magnetic flux)
    static const double Tesla       = Volt*sec/sqm;       ///<@brief Tesla (magnetic field)
    static const double T           = Tesla;              ///<@brief Tesla (magnetic field)
    static const double Gauss       = Tesla/10000.;       ///<@brief Gauss (magnetic field)
    static const double G           = Gauss;              ///<@brief Gauss (magnetic field)
    static const double kG          = G*1000.;            ///<@brief Kilogauss (magnetic field)
    static const double Henry       = Weber/Ampere;       ///<@brief Henry (magnetic inductance)
    ///@}
    ///@name Temperature
    ///@{
    static const double Kelvin      = 1.0;                           ///<@brief Kelvin
    static const double degK        = Kelvin;                        ///<@brief Kelvin
    ///@}
    static const double c          = 1.0;                 ///<@brief Speed of light
    static const double c_si       = 299792458*m/sec;     ///<@brief Speed of light in SI units
    static const double pi         = 2.*std::asin(1.);    ///<@brief Pi
    static const double twopi      = 2.*pi;               ///<@brief 2*pi
    static const double rad        = 1.0;                 ///<@brief Radian
      static const double millirad   = rad/1000.;           ///<@brief Milliradian
    static const double deg        = rad*pi/180.;         ///<@brief Degree
    ///@}
    //    } // Values
    ///@brief Unit names
    namespace Name { 
      ///@name Energy units
      ///@{
      static const std::string MeV = "MeV";        ///<@brief Megaelectronvolt
      static const std::string keV = "keV";        ///<@brief Kiloelectronvolt
      static const std::string eV  = "eV";         ///<@brief ElectronVolt
      static const std::string GeV = "GeV";        ///<@brief Gigaelectronvolt
      static const std::string TeV = "TeV";        ///<@brief Terraelectronvolt
      ///@}
      ///@name Length
      ///@{
      static const std::string mm  = "mm";       ///<@brief Millimeter
      static const std::string mu  = "#mum";     ///<@brief Micrometer
      static const std::string nm  = "nm";       ///<@brief Nanometer
      static const std::string pm  = "pm";       ///<@brief Picometer
      static const std::string fm  = "fm";       ///<@brief Femtometer 
      static const std::string cm  = "cm";       ///<@brief Centimeter
      static const std::string m   = "m";        ///<@brief Meter
      static const std::string km  = "km";       ///<@brief Kilometer
      static const std::string in  = "in";       ///<@brief Inch
      static const std::string ft  = "ft";       ///<@brief Foot
      static const std::string yd  = "yd";       ///<@brief Yard
      static const std::string mi  = "mi";       ///<@brief Mile
      static const std::string F   = "Fermi";    ///<@brief Fermi
      static const std::string Ang = "#AA";      ///<@brief Angstroem
      ///@}
      ///@name Time
      ///@{
      static const std::string sec      = "s";          ///<@brief Second
      static const std::string min      = "m";          ///<@brief Minute
      static const std::string hr       = "h";          ///<@brief Hour
      static const std::string day      = "d";          ///<@brief Day
      static const std::string wk       = "wk";         ///<@brief Week
      static const std::string yr       = "yr";         ///<@brief Year
      static const std::string mo       = "mo";         ///<@brief Month (average for 12 months/year)
      static const std::string millisec = "ms";         ///<@brief Millisecond
      static const std::string microsec = "#mus";       ///<@brief Microsecond
      static const std::string nanosec  = "ns";         ///<@brief Nanosecond
      static const std::string picosec  = "ps";         ///<@brief Picosecond
      static const std::string femtosec = "fs";         ///<@brief Femtosecond
      static const std::string attosec  = "as";         ///<@brief Attosecond
      ///@}
      ///@name Areas
      ///@{
      static const std::string sqmm      = "mm^{2}";           ///<@brief Squaremillimeter 
      static const std::string sqcm      = "cm^{2}";           ///<@brief Squarecentimeter
      static const std::string sqm       = "m^{2}";            ///<@brief Squaremeter
      static const std::string sqkm      = "km^{2}";           ///<@brief Squarekilometer
      static const std::string sqin      = "in^{2}";           ///<@brief Squareinch
      static const std::string sqft      = "ft^{2}";           ///<@brief Squarefoot
      static const std::string sqmi      = "mi^{2}";           ///<@brief Squaremile
      static const std::string b         = "b";                ///<@brief Barn
      static const std::string millib    = "mb";               ///<@brief Millibarn
      static const std::string microb    = "#mub";             ///<@brief Microbarn
      static const std::string nanob     = "nb";               ///<@brief Nanobarn
      static const std::string picob     = "pb";               ///<@brief Picabarn
      static const std::string femtob    = "fb";               ///<@brief Femtobarn
      static const std::string attob     = "ab";               ///<@brief Attobarn
      ///@}
      ///@name Charge
      ///@{
      static const std::string eplus      = "e";                 ///<@brief Positron charge
      static const std::string eplus_si   = "e_{SI}";            ///<@brief Positron charge in SI units
      static const std::string e          = "e";                 ///<@brief Positron charge
      static const std::string Coulomb    = "C";                 ///<@brief Coulomb
      static const std::string C          = "C";                 ///<@brief Coulomb
      ///@}
      ///@name Force, energy, power, mass, pressure in SI units
      ///@{ 
      static const std::string Joule       = "J";         ///<@brief Joule
      static const std::string J           = "J";         ///<@brief Joule
      static const std::string Watt        = "W";         ///<@brief Watt
      static const std::string W           = "W";         ///<@brief Watt
      static const std::string kiloWatt    = "kW";        ///<@brief Kilowatt
      static const std::string kW          = "kW";        ///<@brief Kilowatt
      static const std::string MegaWatt    = "MW";        ///<@brief Megawatt
      static const std::string MW          = "MW";        ///<@brief Megawatt
      static const std::string Newton      = "N";         ///<@brief Newton
      static const std::string N           = "N";         ///<@brief Newton
      static const std::string kg          = "kg";        ///<@brief Kilogram
      static const std::string g           = "g";         ///<@brief Gram
      static const std::string millig      = "mg";        ///<@brief Milligram
      static const std::string microg      = "#mug";      ///<@brief Microgram
      static const std::string Pascal      = "Pa";        ///<@brief Pascal
      static const std::string P           = "Pa";        ///<@brief Pascal
      static const std::string hectoPascal = "hPa";       ///<@brief Hectopascal
      static const std::string hectoP      = "hPa";       ///<@brief Hectopascal
      static const std::string bar         = "bar";       ///<@brief Bar
      static const std::string millibar    = "millibar";  ///<@brief Millibar
      static const std::string atmo        = "atm";       ///<@brief Atmosphere
      ///@}
      ///@name Electrical units
      ///@{
      static const std::string Ampere      = "A";        ///<@brief Ampere (current)
      static const std::string A           = "A";        ///<@brief Ampere (current)
      static const std::string milliA      = "mA";       ///<@brief Milliampere (current)
      static const std::string microA      = "#muA";     ///<@brief Microampere (current)
      static const std::string nanoA       = "nA";       ///<@brief Nanoampere (current)
      static const std::string picoA       = "pA";       ///<@brief Picoampere (current)
      static const std::string Volt        = "V";        ///<@brief Volt (potential)
      static const std::string V           = "V";        ///<@brief Volt (potential)
      static const std::string milliV      = "mV";       ///<@brief Millivolt (potential)
      static const std::string kV          = "kV";       ///<@brief Kilovolt (potential)
      static const std::string MV          = "MV";       ///<@brief Megavolt (potential)
      static const std::string Ohm         = "#Omega";   ///<@brief Ohm (resistance)
      static const std::string kiloOhm     = "k#Omega";  ///<@brief Kiloohm (resistance)
      static const std::string MegaOhm     = "M#Omega";  ///<@brief Megaohm (resistance)
      static const std::string Farad       = "F";        ///<@brief Farad (capacitance)
      static const std::string milliFarad  = "mF";       ///<@brief Millifarad (capacitance)
      static const std::string microFarad  = "#muF";     ///<@brief Microfarad (capacitance)
      static const std::string nanoFarad   = "nF";       ///<@brief Nanofarad (capacitance)
      static const std::string picoFarad   = "pF";       ///<@brief Picofarad (capacitance)
      static const std::string Weber       = "Wb";       ///<@brief Weber (magnetic flux)
      static const std::string Tesla       = "T";        ///<@brief Tesla (magnetic field)
      static const std::string T           = "T";        ///<@brief Tesla (magnetic field)
      static const std::string Gauss       = "G";        ///<@brief Gauss (magnetic field)
      static const std::string G           = "G";        ///<@brief Gauss (magnetic field)
      static const std::string kG          = "kG";       ///<@brief Kilogauss (magnetic field)
      static const std::string Henry       = "H";        ///<@brief Henry (magnetic inductance)
      ///@}
      ///@name Temperature
      ///@{
      static const std::string Kelvin      = "^{#circ}K"; ///<@brief Kelvin
      static const std::string degK        = "^{#circ}K"; ///<@brief Kelvin
      static const std::string Celsius     = "^{#circ}C"; ///<@brief Celsius
      static const std::string Fahrenheit  = "^{#circ}F"; ///<@brief Fahrenheit
      ///@}
      static const std::string c          = "c";          ///<@brief Speed of light
      static const std::string c_si       = "c_{SI}";     ///<@brief Speed of light in SI units
      static const std::string pi         = "#pi";        ///<@brief Pi
      static const std::string twopi      = "2#pi";       ///<@brief 2*pi
      static const std::string rad        = "rad";        ///<@brief Radian
      static const std::string millirad   = "mrad";       ///<@brief Milliradian
      static const std::string deg        = "^{#circ}";   ///<@brief Degree
      ///@}
    } // Name
  } // Units
  ///@brief Non-trivial unit conversions
  namespace Converters {
    ///@brief Temperatures
    namespace Temperature { 
      ///@brief Convert to Kelvin
      struct Kelvin { 
	static double fromCelsius(double t)    { return t+273.15; }            ///<@brief From Celsius
	static double fromFahrenheit(double t) { return (t + 459.67)*5./9.; }  ///<@brief From Fahrenheit
      };
      ///@brief Convert to Celsius
      struct Celsius {
	static double fromKelvin(double t)     { return t-273.15; }            ///<@brief From Kelvin
	static double fromFahrenheit(double t) { return (t-32.)*5./9.; }       ///<@brief From Fahrenheit
      };
      ///@brief Convert to Fahrenheit 
      struct Fahrenheit {
	static double fromKelvin(double t)  { return t*9./5.-459.67; }         ///<@brief From Kelvin
	static double fromCelsius(double t) { return t*9./5.+32.; }            ///<@brief From Celsius
      };
    } // Temperature
  } // Converters
} // Analysis

#define ANA_TEMP_CNV( X, Y, FROM, TO )  Y = Analysis::Converters::Temperature::##TO::from##FROM( X )
#define ANA_UNIT_NAME( UNIT )           Analysis::Units::Name::##UNIT
#define ANA_ASS_UNIT_NAME( X, UNIT )    X = ANA_UNIT_NAME( UNIT )
#define ANA_UNIT_VALUE( UNIT )          Analysis::Units::Value::##UNIT
#define ANA_APP_UNIT_VALUE( X, Y, UNIT) Y = X * ANA_UNIT_VALUE( UNIT )

#endif
