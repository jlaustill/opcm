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
};