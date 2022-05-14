#include <iostream>
#include <pqxx/pqxx>
#include <fstream>
#include "exerciser.h"

using namespace std;
using namespace pqxx;

void creatTable(connection *C){
  work W(*C);
  string drop;
  string player, team, state, color;
  drop = "DROP TABLE IF EXISTS COLOR, STATE, TEAM, PLAYER;";
  color = 
            "CREATE TABLE IF NOT EXISTS COLOR(" \
            "COLOR_ID     SERIAL      NOT NULL," \
            "NAME         VARCHAR(30) NOT NULL," \
            "PRIMARY KEY (COLOR_ID));";

  state = 
            "CREATE TABLE IF NOT EXISTS STATE(" \
            "STATE_ID     SERIAL      NOT NULL," \
            "NAME         VARCHAR(30) NOT NULL," \
            "PRIMARY KEY (STATE_ID));";

  team = 
            "CREATE TABLE IF NOT EXISTS TEAM(" \
            "TEAM_ID      SERIAL      NOT NULL," \
            "NAME         VARCHAR(30) NOT NULL," \
            "STATE_ID     SERIAL      NOT NULL," \
            "COLOR_ID     SERIAL      NOT NULL," \
            "WINS          INT         NOT NULL," \
            "LOSSES       INT         NOT NULL," \
            "PRIMARY KEY (TEAM_ID)," \
            "FOREIGN KEY (STATE_ID) REFERENCES STATE(STATE_ID)," \
            "FOREIGN KEY (COLOR_ID) REFERENCES COLOR(COLOR_ID));" ;

  player =  
            "CREATE TABLE IF NOT EXISTS PLAYER("  \
            "PLAYER_ID    SERIAL      NOT NULL," \
            "TEAM_ID      SERIAL      NOT NULL," \
            "UNIFORM_NUM  INT      NOT NULL," \
            "FIRST_NAME   VARCHAR(30) NOT NULL," \
            "LAST_NAME    VARCHAR(30) NOT NULL," \
            "MPG          INT     NOT NULL," \
            "PPG          INT     NOT NULL," \
            "RPG          INT     NOT NULL," \
            "APG          INT     NOT NULL," \
            "SPG          REAL     NOT NULL," \
            "BPG          REAL     NOT NULL," \
            "PRIMARY KEY (PLAYER_ID)," \
            "FOREIGN KEY (TEAM_ID) REFERENCES TEAM(TEAM_ID));";
  
  W.exec(drop);
  W.exec(color);
  W.exec(state);
  W.exec(team);
  W.exec(player);
  W.commit();
}
int main (int argc, char *argv[]) 
{

  //Allocate & initialize a Postgres connection object
  connection *C;

  try{
    //Establish a connection to the database
    //Parameters: database name, user name, user password
    C = new connection("dbname=ACC_BBALL user=postgres password=passw0rd");
    if (C->is_open()) {
      cout << "Opened database successfully: " << C->dbname() << endl;
    } else {   
      cout << "Can't open database" << endl;
      return 1;
    }
  } catch (const std::exception &e){
    cerr << e.what() << std::endl;
    return 1;
  }



  //TODO: create PLAYER, TEAM, STATE, and COLOR tables in the ACC_BBALL database
  creatTable(C);
  

  //      load each table with rows from the provided source txt files
  string line;
  ifstream fd;

  string colorID, colorName;
    fd.open("./color.txt", ios::in);
    while(getline(fd, line)){
        stringstream ss;
        ss << line;
        ss >> colorID >> colorName;
        add_color(C, colorName);
    }
    fd.close();

  string stateID, stateName;
  fd.open("./state.txt", ios::in);
  while(getline(fd, line)){
      stringstream ss;
      ss << line;
      ss >> stateID >> stateName;
      add_state(C, stateName);
  }
  fd.close();

  string teamID, teamName, ref_stateID, ref_colorID, wins_s, losses_s;
  fd.open("./team.txt", ios::in);
  while(getline(fd, line)){
      stringstream ss;
      ss << line;
      ss >> teamID >> teamName >> ref_stateID >> ref_colorID >> wins_s >> losses_s;
      add_team(C, teamName, stoi(ref_stateID), stoi(ref_colorID), stoi(wins_s), stoi(losses_s));
  }
  fd.close();

  string playerID, ref_teamID, uniNum, firstName, lastName, mpgs, ppgs, rpgs, apgs, spgs, bpgs;
  fd.open("./player.txt", ios::in);
  while(getline(fd, line)){
      stringstream ss;
      ss << line;
      ss >> playerID >> ref_teamID >> uniNum >> firstName >> lastName >> mpgs >> ppgs >> rpgs >> apgs >> spgs >> bpgs;
      add_player(C, stoi(ref_teamID), stoi(uniNum), firstName, lastName, stoi(mpgs), stoi(ppgs), stoi(rpgs), stoi(apgs), stod(spgs), stod(bpgs));
  }
  fd.close();
  

  
  exercise(C);


  //Close database connection
  C->disconnect();

  return 0;
}


