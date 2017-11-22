#ifndef TIMER_H
#define TIMER_H

#include <thread>           //C++ includes built-in support for threads, mutual exclusion, condition variables, and futures. Multithreading Purpose.
#include <chrono>           //Used for very precise Time Measurements
using namespace std;
/*
Multiple threads can run simultaneously. This is important because we want to send our mail simultaneously while recording Keystrokes.
Like, in the main function, the GetMessage was out main thread.
*/

class Timer
    {

     //Private members

    thread Thread;         //Creates a variable thread from the library thread for asynchronous code execution without blocking the main thread.

    bool Alive = false;         //Indicates State of the timer. If it is running or not

    long CallNumber = -1L;      //It will determine how many times we will like to call out functions. -1L is arbitrary. We're using L because this program
                                //can run for over an year and we don't want the overflow to happen

    long repeat_count = -1L;    //Count the number of times a certain function has already been called

    chrono::milliseconds interval = chrono::milliseconds(0);  //This will represent the interval between function calls

    function<void(void)> funct = nullptr;   //doesn't point to any function, no return type and no inputs
    //This is a new concept in C++11. function is a type that can store functions, similar to pointer to a function.
    //In the case the function funct represents a function with no arguments or return type, and currently pointing to null
    //funct is the function thread will be running.

    void SleepAndRun ()
        {



        this_thread::sleep_for(interval);

        //sleep_for will block the thread for a certain time
        //this_thread indicates the current thread which created the timer object.
        //above statement will pause the execution of thread for given interval

        if (Alive)
            Function ()();                            //If the timer is running, call the function, initially bool was set to 0
        }                                             //Two parenthesis because first parenthesis will call the function and second parenthesis will call the
                                                      //function that this function calls.


    void ThreadFunc ()
        {                                             //If the number of times the function has to be called is infinite, we will keep calling the function till the thread is alive


        if (CallNumber == Infinite)
            while (Alive)
                SleepAndRun ();                       //A function defined above
        else



            while (repeat_count--)                    //Reducing the repeat_count. Every positive number will evaluate a truth, once it becomes , evaluates false
                SleepAndRun ();                       //A function defined above
        }

//Public members


public:
    static const long Infinite = -1L;                // We will use it state that a function will be called infinite amount of times so the timer would work until
                                                     // we stop it in a manual fashion. It is static so that we can use it without creating an instance

    Timer () {}                                      //Empty constructor so that we can create as a global variable //default constructor

    Timer(const function<void(void)> &f) : funct (f) {}   //This constructor will accept a pointer to a function that we will call. We will initialize the function
                                                               //object to f
                                                               //simply a constructor that takes a function as parameter


    Timer(const function<void(void)> &f,const unsigned long &i,const long repeat = Timer::Infinite)           //This will have more parameters

      {
          funct (f);
          interval=chrono::milliseconds(i);
          CallNumber = repeat;
      }

     //This will start the execution of the thread.
     //Parameter indicates whether thread runs asynchronously or not.

    void Start (bool Async = true)                     //For starting the timer
        {

        if (IsAlive ())
            return;                                    //If it's already running, just return we don't have to do anything


                                                       //initialize variables at starting of thread.
        Alive = true;                                  //Set's alive as true


        repeat_count = CallNumber;                     //Sets the value of repeat_count to CallNumber


        if (Async)                                //If async, we need to create a new thread for the function using 'this' Timer object
                                                  //execution of this thread is independent of main thread and may be done simultaneously.
            Thread = thread (ThreadFunc, this);   //Thread is our variable. ThreadFunc will be executed and this is the pointer to timer object

                                                  //if it is sync, we need to block the main thread and first complete this thread.
                                                  //execution of main thread depends on this thread so we block it till this thread is done.
        else
            this->ThreadFunc ();
        }

    void Stop ()                                       //To stop the execution of thread.
        {

        Alive = false;


        Thread.join ();                                 //No concurrent execution on the one main thread.
                                                        //Thread.join() reconnects the current thread with main thread.
                                                        //current thread was created as a branch of main thread.
                                                        //so it should be rejoined when execution of this thread completed.


        }

    void SetFunction (const function<void (void)> &f)    //used to set the function that will be executed.
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

    void SetInterval (const unsigned long &i)               //set function for interval between iterations.
        {

        if (Alive)
            return;;

        interval = chrono::milliseconds(i);
        }

    unsigned long Interval () const {return interval.count();}     //.count is there to get a number from the interval

    const function<void(void)> &Function () const             //Function returns a function 'funct'
        {                                                     //so when we write Function()(), it executes funct()

        return funct;
        }
    };

#endif
