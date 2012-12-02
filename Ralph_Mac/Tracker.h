/*
 *  Tracker.h
 *  RALPH
 *
 *  Created by Ronald on 14/05/11.
 *
 */

struct DATA {

	unsigned char* mdata;
	long size;
	int channel;
	int depth;
	int widthStep;
};
int StartTracking();
int updateTracker(int);
int getdisturbances();

DATA* getData();
void deleteafter();
void pushtoStore(DATA* mdata);
int SendData(DATA* data);
int WebserverCaller();
int closeall();