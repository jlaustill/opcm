struct addresses {
    int odometer = 0;
    int tripA = 8;
    int tripB = 16;
    int count = 24;
};

extern struct addresses opcmMemoryAddresses;

class memory {
    private:

    public:
    static double getOdometer();
    static void setOdometer(double newReading);
    static double getTripA();
    static void setTripA(double newReading);
    static double getTripB();
    static void setTripB(double newReading);
    static double getSaveCount();
    static void setSaveCount(unsigned long newReading);
};