// this is a struct built to represent a process
typedef struct process {
	int arrival;
	int burst;
	int finishTime;
	int turnAroundTime;
	float normalizedTurnAround;
	int timeRemaining;
	int finished;
	int latency;
	
}Process;
