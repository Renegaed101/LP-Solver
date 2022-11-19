#include <stdexcept>
#include <numeric> 
#include <cmath> 

class fraction;
std::ostream& operator<< (std::ostream& stream, fraction const & a);

class fraction {
    private:
        int numerator;
        int denominator;


        //Default constructor is delegated to 2-argument constructor
        fraction (): fraction{0,1} {}
        
        //Personal implemention of greatest common divisor, however std library function is 
        //probably better
        /*
        int gcd (int a, int b) {
            int n {a};
            
            while (n > 1) {
                if (a%n == 0 && b%n == 0) {
                    return n;
                }
                n--;
            }
            return 1;
        }
        */


        //Used in previous version, this logic has been moved into the constructor
        fraction lowestTerms () const {
            int divisor = std::gcd (numerator,denominator);       
            return fraction {numerator/divisor, denominator/divisor}; 
        }
        
    public:

        fraction (double input) {
        
        double integral = std::floor(input);
        double frac = input - integral;

        int precision = 1000000000; // This is the accuracy.

        int gcd_ = std::gcd((int)(std::round(frac * precision)), precision);

        int denominator = precision / gcd_;
        int numerator = round(frac * precision) / gcd_;
        
        this->numerator = integral * denominator + numerator;
        this->denominator = denominator;

        }        
        
        fraction (int numerator,int denominator) {
            if (denominator == 0) {
                throw std::invalid_argument {"Denominator must be non-zero"};
            }
            if (denominator < 0) {
                denominator = -denominator;
                numerator = -numerator;
            }

            int divisor = std::gcd(numerator,denominator);
            this->numerator = numerator/divisor;
            this->denominator = denominator/divisor;
        }
        
        //Copy constructor, when a new Point is being created as a copy of an existing non-temporary Point object
        //Note compiler provides one anyway if not specified, included here to track if unnecessary copies are being made
        fraction(fraction const & other) {
            //std::cout << "Copy constructor Initialized for " << other << std::endl;
            numerator = other.numerator;
            denominator = other.denominator;
        }

        double toReal () const{
            return numerator/(double)denominator;
        }

        int getNumerator () const {
            return numerator;
        } 

        int getDenominator () const {
            return denominator;
        }

        //Note that when defining a member method, the first parameter is the object itself
        //Also note that you have access to others private members, as we are within the point class
        

        //Unary minus, for negation e.g -a
        fraction operator- () const {
            return fraction {-numerator,denominator};
        }

        fraction operator+ (fraction const & other) const {
            int a = numerator, b = denominator, c = other.numerator, d = other.denominator;
            return (fraction {a*d + b*c, b * d});
        }

        fraction operator- (fraction const & other) const {
            int a = numerator, b = denominator, c = other.numerator, d = other.denominator;
            return (fraction {a*d - b*c, b * d});
        }

        fraction operator* (fraction const & other) const {
            int a = numerator, b = denominator, c = other.numerator, d = other.denominator;
            return (fraction {a*c, b*d});
        }

        fraction operator* (double const & other) const {
            return (*this * fraction {other} );
        }

        fraction operator* (int const & other) const {
            return (*this * fraction {other,1} );
        }

        fraction operator/ (fraction const & other) const {
            int a = numerator, b = denominator, c = other.numerator, d = other.denominator;
            return (fraction {a*d, b*c});
        }       

        bool operator< (fraction const & other) {
            int a = numerator, b = denominator, c = other.numerator, d = other.denominator;
            return a*d < b*c;
        }

        fraction& operator= (std::pair<int,int> p) {
            numerator = p.first;
            denominator = p.second;
            return *this;
        } 

        //This is a conversion operator, allowing a function to be assigned to a double
        operator double() const {
            return toReal();
        }

 
};



//Note that this operator function needs to remain outside of the point class, because the left hand side
//of this operator needs to be of type stream, while within the point class, each left parameter of a 
//member function is the point itself, this could be written in the stream class, but we don't have access to it
std::ostream& operator<< (std::ostream& stream, fraction const & a) {
    stream << a.getNumerator() << "/" << a.getDenominator();
    return stream;   
}

