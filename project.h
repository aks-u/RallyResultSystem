#ifndef _PROJECT__H_
#define _PROJECT__H_

void addDriver(char *surname, char *team);
void addDriverList(char *surname, char *team, int hour, int minute, int second);
void updateTime(char *surname, int hour, int minute, int second);
void printRace();
void saveToFile(char *filename);
void loadFromFile(char *filename);
void freeList();
int main(void);

#endif //! _PROJECT__H_