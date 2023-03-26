struct addresses {
    int odometer = 0;
    int tripA = 8;
    int tripB = 16;
    int count = 24;
    int oilChange = 32;
    int transmissionFluidChange = 40;
    int transferCaseFluidChange = 48;
    int frontDifferentialFluidChange = 56;
    int rearDifferentialFluidChange = 64;
    int fuelFilterChange = 72;
    int tireRotation = 80;
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
    static double getOilChange();
    static void setOilChange(unsigned long newReading);
    static double getTransmissionFluidChange();
    static void setTransmissionFluidChange(unsigned long newReading);
    static double getTransferCaseFluidChange();
    static void setTransferCaseFluidChange(unsigned long newReading);
    static double getFrontDifferentialFluidChange();
    static void setFrontDifferentialFluidChange(unsigned long newReading);
    static double getRearDifferentialFluidChange();
    static void setRearDifferentialFluidChange(unsigned long newReading);
    static double getFuelFilterChange();
    static void setFuelFilterChange(unsigned long newReading);
    static double getTireRotation();
    static void setTireRotation(unsigned long newReading);
};