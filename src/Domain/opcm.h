class opcm
{
private:
    static unsigned long count;
    static unsigned long lastMillis;
    static unsigned long thisMillis;
    static unsigned long thisDuration;
    static float thisMileage;
    static long sweep;
    static long maxSweep;
    static int up;

    static void newSweepValue();

public:
    static void setup();
    static void loop();
};
