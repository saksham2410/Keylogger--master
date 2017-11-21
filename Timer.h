#ifndef TIMER_H
#define TIMER_H

#include <thread>           //C++ includes built-in support for threads, mutual exclusion, condition variables, and futures. Multithreading Purpose.
#include <chrono>           //Used for very precise Time Measurements

/*
Multiple threads can run simultaneously. This is important because we want to send our mail simultaneously while recording Keystrokes.
Like, in the main function, the GetMessage was out main thread.
*/

class Timer
    {



    std::thread Thread;         //Creates a variable thread from the library thread for asynchronous code execution without blocking the main thread.

    bool Alive = false;         //Indicates State of the timer. If it is running or not

    long CallNumber = -1L;      //It will determine how many times we will like to call out functions. -1L is arbitrary. We're using L because this program
                                //can run for over an year and we don't want the overflow to happen

    long repeat_count = -1L;    //Count the number of times a certain function has already been called

    std::chrono::milliseconds interval = std::chrono::milliseconds(0);  //This will represent the interval between function calls

    std::function<void(void)> funct = nullptr;   //doesn't point to any function, no return type and no inputs

    void SleepAndRun ()
        {



        std::this_thread::sleep_for(interval);        //sleep_for will block the thread for a certain time
        if (Alive)
            Function ()();                            //If the timer is running, call the function, initially bool was set to 0
        }                                             //Two parenthesis because first parenthesis will call the function and second parenthesis will call the
                                                      //function that this function calls.
    void ThreadFunc ()                                //This will be the function that is passed to the thread
        {


        if (CallNumber == Infinite)
            while (Alive)
                SleepAndRun ();                       //A function defined above
        else



            while (repeat_count--)                    //Reducing the repeat_count. Every positive number will evaluate a truth, once it becomes , evaluates false
                SleepAndRun ();                       //A function defined above
        }


public:
    static const long Infinite = -1L;                // We will use it state that a function will be called infinite amount of times so the timer would work until
                                                     // we stop it in a manual fashion. It is static so that we can use it without creating an instance

    Timer () {}                                      //Empty constructor so that we can create as a global variable

    Timer(const std::function<void(void)> &f) : funct (f) {}   //This constructor will accept a pointer to a function that we will call. We will initialize the function
                                                               //object to f
    Timer(const std::function<void(void)> &f,                  //This will have more parameters
          const unsigned long &i,
          const long repeat = Timer::Infinite) : funct (f),
                                                 interval (std::chrono::milliseconds(i)), CallNumber (repeat) {}



    void Start (bool Async = true)                     //For starting the timer
        {

        if (IsAlive ())
            return;                                    //If it's already running, just return we don't have to do anything

        Alive = true;                                  //Set's alive as true

        repeat_count = CallNumber;                     //Set's the value of repeat_count to CallNumber

        if (Async)
            Thread = std::thread (ThreadFunc, this);   //Thread is our variable. ThreadFunc will be executed and this is the pointer to our class
        else
            this->ThreadFunc ();
        }

    void Stop ()                                       //To stop the timer
        {

        Alive = false;


        Thread.join ();                               //No concurrent execution on the one main thread.
        }
    void SetFunction (const std::function<void (void)> &f)    //used to set the function that will be executed.
        {
        funct = f;
        }
    bool IsAlive () const {return Alive;}             //knows whether the times is alive or not
    void RepeatCount (const long r)                   //it sets the number of repeatances
        {

        if (Alive)
            return;
        CallNumber = r;                               //set's the number of repeatances.
        }

    long GetLeftCount () const {return repeat_count;}        //How many iterations are left. For iterations.

    long RepeatCount () const {return CallNumber;}          //Fetch us the repeat_count. Total number of repeatances. For that we have set. Doesn't change
    void SetInterval (const unsigned long &i)               //Receives a parameter and based on that parameter it just sets the interval
        {

        if (Alive)
            return;;

        interval = std::chrono::milliseconds(i);
        }

    unsigned long Interval () const {return interval.count();}     //.count is there to get a number from the interval

    const std::function<void(void)> &Function () const             //
        {

        return funct;
        }
    };

#endif
