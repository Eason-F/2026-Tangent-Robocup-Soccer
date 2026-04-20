#include <irseeker/IRSeeker.hpp>

QIKEasy::QIKEasy(TwoWire &wirePort) {
    wirePort.begin();
}