/* 

    Program in C++ to implement Selective Repeat ARQ in noisy channel 

*/
#include<iostream>
#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<time.h>
#include<Windows.h>

using namespace::std;

void Send_frames(int,int);
int Receive();
void Resend_lost_frame(int);
void Resend_frames_due_to_timeout(int,int);

int main()
{
    srand(time(0));
    int i,window_size=8,number_of_frames,remaining_number_of_frames;
    bool timeout = false;      

    cout<<"Enter the number of frames: ";
    cin>>number_of_frames;
    cout<<"Window size is : "<<window_size;
    cout<<"\n-----------------------------------\n\n";
    remaining_number_of_frames = number_of_frames;
    
    int start_frame = 0;
    while (remaining_number_of_frames!=0)
    {
        timeout = false;
        if(remaining_number_of_frames < window_size) {
            window_size = remaining_number_of_frames;
        }
        cout<<"\nWindow is: "<<start_frame<<" to "<<start_frame+window_size;
        Send_frames(start_frame,window_size);
        
        receive:
        clock_t time_start = clock();
        int x = Receive();     //Receiver should receive the frames of the window
        clock_t time_end = clock();

        if((time_end - time_start)/CLOCKS_PER_SEC > 5)
        {
            if(!timeout) {
                cout<<"\n\tTimeout...\n"<<"\tResending window again";   //Timeout
                Resend_frames_due_to_timeout(start_frame,window_size);
            }
            timeout = true;
            goto receive;
        }
        else if(x == 0) {
            int frame_to_be_resent = rand()%(window_size) + start_frame;
            cout<<"\n\tNAK "<<frame_to_be_resent;       //Negative Acknowledgement
            cout<<"\nTime taken = "<<(double)(time_end - time_start)/CLOCKS_PER_SEC<<" secs";
            Resend_lost_frame(frame_to_be_resent);
            timeout = true;
            goto receive;
        }
        else {
            cout<<"\n\tACK";    //Acknowledged
            cout<<"\nTime taken = "<<(double)(time_end - time_start)/CLOCKS_PER_SEC<<" secs";
        }

        start_frame += 8;
        remaining_number_of_frames -= window_size;
    }

    return 0;

}


//Function to send the frames
void Send_frames(int start_frame,int window_size) {
    for(int i=start_frame;i<start_frame+window_size;i++) {
        cout<<"\nSending frame " <<i;
        Sleep(500);
    }
}

// Function that simulates the receiver 
int Receive() {
    int n = rand()%6+1;     //Generate a random number between 1 and 6
    Sleep(n*1000);          //Sleep for 1,2,3,4,5 or 6 seconds
    return rand()%2;    //0 or 1 randomly where 1=>ACK and 0=>NAK
}

//Function to resend the frames due to timeout
void Resend_frames_due_to_timeout(int start_frame, int window_size) {
    cout<<"\nResending the frames "<<start_frame<<" to "<<start_frame+window_size;
    for(int i=start_frame;i<start_frame+window_size;i++){
        cout<<"\nSending frame " <<i;
        Sleep(500);
    }
}

//Function to resend the frame lost (NAK)
void Resend_lost_frame(int frame_number) {
    cout<<"\nResending the frame "<<frame_number;
    Sleep(1000);
}