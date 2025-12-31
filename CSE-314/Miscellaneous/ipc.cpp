#include <chrono>
#include <fstream>
#include <iostream>
#include <pthread.h>
#include <random>
#include <unistd.h>
#include <vector>
#include <semaphore.h>

#define GALLERY1_CAPACITY 5
#define STAIR_COUNT 3
#define CORRIDOR_CAPACITY 3
#define STAIR_SLEEP 1
#define CORRIDOR_SLEEP 20
#define MULTIPLIER 1000
#define ORDINARY_START 1001
#define PREMIUM_START 2001

#define COLOR_RESET  string("\x1B[0m")
#define COLOR_YELLOW string("\x1B[33m")
#define COLOR_CYAN   string("\x1B[36m")
#define COLOR_GREEN  string("\x1B[32m")
#define COLOR_RED    string("\x1B[31m")
#define COLOR_PURPLE string("\x1B[35m")
#define COLOR_BLUE   string("\x1B[34m")


using namespace std;
using namespace chrono;



int N,M;
int w,x,y,z;

auto start_time = high_resolution_clock::now();
class Visitor{
    public:
    int id;
    int type;// 0 for standard,1 for premium

    Visitor(int id,int type) : id(id),type(type){
    }
};

vector<pthread_mutex_t> stair_locks(STAIR_COUNT);
vector<Visitor> visitors;
sem_t gallery1_sem;
sem_t corridor_sem;
int premium_count;
int standard_count;
pthread_mutex_t photo_booth_lock;
pthread_mutex_t priority_lock;
pthread_mutex_t standard_count_lock;
pthread_mutex_t premium_count_lock;

int get_random_number(){
    random_device rdev;
    mt19937 generator(rdev());
    double lambda=12345.678;
    poisson_distribution<int> poisson_dist(lambda);
    return poisson_dist(generator);
}


void initialize(){
    for(int i=0;i<N;i++){
        visitors.emplace_back(Visitor(ORDINARY_START+i,0));
    }
    for(int i=0;i<M;i++){
        visitors.emplace_back(Visitor(PREMIUM_START+i,1));
    }
    for(int i=0;i<STAIR_COUNT;i++){
        pthread_mutex_init(&stair_locks[i],NULL);
    }
    sem_init(&gallery1_sem,0,GALLERY1_CAPACITY);
    sem_init(&corridor_sem,0,CORRIDOR_CAPACITY);
    
    premium_count=0;
    standard_count=0;
    
    pthread_mutex_init(&photo_booth_lock,NULL);
    pthread_mutex_init(&priority_lock,NULL);
    pthread_mutex_init(&standard_count_lock,NULL);
    pthread_mutex_init(&premium_count_lock,NULL);

    start_time= high_resolution_clock::now();
}

long long get_time(){
    auto cur_time=high_resolution_clock::now();
    auto duration=duration_cast<milliseconds> (cur_time-start_time);
    int tim_ms = duration.count();
    return tim_ms;
}

// uses mutex lock to write output to avoid interleaving
void enter_into_hallway(string output){
    int delay=get_random_number()%10+1;
    // cout<<delay< COLOR_RESET<+"\n";
    usleep(MULTIPLIER*delay);
    cout<<output+to_string(get_time()) + COLOR_RESET +"\n";
}
void spend_time_in_hallway(string output){

    usleep(MULTIPLIER*w);
    cout<<output+to_string(get_time()) + COLOR_RESET +"\n";
}
void jump_to_first_stair(string output){
    pthread_mutex_lock(&stair_locks[0]);
    cout << output+to_string(get_time()) + COLOR_RESET +"\n";
    usleep(MULTIPLIER*STAIR_SLEEP); 
}

void jump_on_stair(int stairno,string output) {
    pthread_mutex_lock(&stair_locks[stairno]);
    cout << output+to_string(get_time()) + COLOR_RESET +"\n";
    pthread_mutex_unlock(&stair_locks[stairno-1]);
    usleep(MULTIPLIER*STAIR_SLEEP);
}

void goto_gallery1(string output){
    sem_wait(&gallery1_sem);
    cout<<output+to_string(get_time()) + COLOR_RESET +"\n";
    pthread_mutex_unlock(&stair_locks[STAIR_COUNT-1]);
    usleep(MULTIPLIER*x);
}

void goto_corridor(string output){
    sem_wait(&corridor_sem);
    cout<<output+to_string(get_time()) + COLOR_RESET +"\n";
    sem_post(&gallery1_sem);
    usleep(MULTIPLIER*CORRIDOR_SLEEP);
}
void goto_gallery2(string output){
    cout<<output+to_string(get_time()) + COLOR_RESET +"\n";
    sem_post(&corridor_sem);
}
void goto_photo_booth(string output){
    
    usleep(MULTIPLIER*y);
    cout<<output+to_string(get_time()) + COLOR_RESET +"\n";
}

void take_premium_photo(string output2){

    pthread_mutex_lock(&premium_count_lock);
    premium_count++;
    // cout<<premium_count<<" wwwwwwww\n";
    if(premium_count==1) pthread_mutex_lock(&priority_lock);
    pthread_mutex_unlock(&premium_count_lock);


    pthread_mutex_lock(&photo_booth_lock);
    cout<<output2+to_string(get_time()) + COLOR_RESET +"\n";
    usleep(MULTIPLIER*z);
    // cout<<output2+" finished "+to_string(get_time())+"\n";
    pthread_mutex_unlock(&photo_booth_lock);


    pthread_mutex_lock(&premium_count_lock);
    premium_count--;
    // cout<<premium_count<<" wwwwwwwwwww\n";
    if(premium_count==0){ 
        // cout<<"photo unlocked at "+to_string(get_time())<<"\n";
        pthread_mutex_unlock(&priority_lock);
    }
    pthread_mutex_unlock(&premium_count_lock);

}

void take_standard_photo(string output2){

    pthread_mutex_lock(&priority_lock);
    pthread_mutex_lock(&standard_count_lock);
    standard_count++;
    // cout<<standard_count<<"\n";
    if(standard_count==1) pthread_mutex_lock(&photo_booth_lock);
    
    pthread_mutex_unlock(&standard_count_lock);
    pthread_mutex_unlock(&priority_lock);

    cout<<output2+to_string(get_time()) + COLOR_RESET +"\n";
    usleep(MULTIPLIER*z);
    // cout<<output2+" finished "+to_string(MULTIPLIER*z)+" "+to_string(get_time()) +"\n";


    pthread_mutex_lock(&standard_count_lock);
    standard_count--;
    // cout<<standard_count<<"\n";
    if(standard_count==0){ 
        // cout<<"photo unlocked at "+to_string(get_time())+"\n";
        pthread_mutex_unlock(&photo_booth_lock);
    }  
    pthread_mutex_unlock(&standard_count_lock);

}



void *start(void *arg){
    Visitor *visitor=(Visitor *)arg;

    enter_into_hallway(COLOR_BLUE+"Visitor " + to_string(visitor->id) +" has arrived at A at timestamp ");
    spend_time_in_hallway(COLOR_CYAN +"Visitor " + to_string(visitor->id) +" has arrived at B at timestamp ");
    jump_to_first_stair(COLOR_GREEN +"Visitor " + to_string(visitor->id) +" is at step 1 at timestamp ");
    jump_on_stair(1,COLOR_GREEN+"Visitor " + to_string(visitor->id) +" is at step 2 at timestamp ");
    jump_on_stair(2,COLOR_GREEN+"Visitor " + to_string(visitor->id) +" is at step 3 at timestamp ");
    goto_gallery1(COLOR_YELLOW+"Visitor " + to_string(visitor->id) +" is at C (entered Gallery 1) at timestamp ");
    goto_corridor(COLOR_PURPLE+"Visitor " + to_string(visitor->id) +" is at D (exiting Gallery 1) at timestamp ");
    goto_gallery2(COLOR_RED+"Visitor " + to_string(visitor->id) +" is at E (entered Gallery 2) at timestamp ");
    goto_photo_booth(COLOR_BLUE+"Visitor " + to_string(visitor->id) +" is about to enter the photo booth at timestamp ");

    if(visitor->type==0){
        take_standard_photo(COLOR_CYAN+"Visitor " + to_string(visitor->id) +" is inside the photo booth at timestamp ");
    }else{
        take_premium_photo(COLOR_RED+"Visitor " + to_string(visitor->id) +" is inside the photo booth at timestamp ");
    }
    cout<<COLOR_YELLOW+"Visitor "+ to_string(visitor->id) +" has left the photo booth at F (exiting Gallery 2) at timestamp "+to_string(get_time()) + COLOR_RESET +"\n";
    return nullptr;
}




int main(){

    cin>>N>>M;
    cin>>w>>x>>y>>z;
    pthread_t visitors_thread[N+M];
    initialize();

    vector<bool> started(N+M);

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, 1024 * 1024);  // Set stack size to 1MB
	pthread_attr_setschedpolicy(&attr,SCHED_RR);	  


    for(int i=0;i<N+M;i++){
        pthread_create(&visitors_thread[i],&attr,&start,&visitors[i]);
    }
    for(int i=0;i<N+M;i++){
        pthread_join(visitors_thread[i],NULL);
    }
    pthread_attr_destroy(&attr);
    return 0;
    
}




