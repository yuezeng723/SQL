#include "query_funcs.h"



void add_player(connection *C, int team_id, int jersey_num, string first_name, string last_name,
                int mpg, int ppg, int rpg, int apg, double spg, double bpg)
{
    work W(*C);
    stringstream insertPlayer;
    string insert =  "INSERT INTO PLAYER (TEAM_ID, UNIFORM_NUM, FIRST_NAME, LAST_NAME, MPG, PPG, RPG, APG, SPG, BPG) VALUES ( "; 
    insertPlayer << insert 
    << team_id << ","
    << jersey_num <<  ","
    << W.quote(first_name)  << ","
    << W.quote(last_name) << ","
    <<mpg  << ","<<ppg << ","<<rpg << ","<<apg << ","<<spg << ","<<bpg <<");";
    W.exec(insertPlayer.str());
    W.commit();
}                                                                                           


void add_team(connection *C, string name, int state_id, int color_id, int wins, int losses)
{
    work W(*C);
    stringstream insertTeam;
    string insert =  "INSERT INTO TEAM(NAME, STATE_ID, COLOR_ID, WINS, LOSSES) VALUES ( "; 
    insertTeam << insert 
    <<"'" << name <<"'" <<","
    << state_id << ","
    << color_id << ","
    << wins << ","
    << losses <<");";
    W.exec(insertTeam.str());
    W.commit();
}


void add_state(connection *C, string name)
{
    work W(*C);
    stringstream insertState;
    string insert =  "INSERT INTO STATE(NAME) VALUES ( "; 
    insertState << insert
    <<"'" << name <<"'" <<");";
    W.exec(insertState.str());
    W.commit();
}


void add_color(connection *C, string name)
{
    work W(*C);
    stringstream insertColor;
    string insert =  "INSERT INTO COLOR (NAME) VALUES ( "; 
    insertColor << insert
    <<"'" <<name <<"'" <<");";
    W.exec(insertColor.str());
    W.commit();
}


void query1(connection *C,
	    int use_mpg, int min_mpg, int max_mpg,
            int use_ppg, int min_ppg, int max_ppg,
            int use_rpg, int min_rpg, int max_rpg,
            int use_apg, int min_apg, int max_apg,
            int use_spg, double min_spg, double max_spg,
            int use_bpg, double min_bpg, double max_bpg
            )
{
    work W(*C);
    string selectAll = "SELECT * FROM PLAYER WHERE ";
    stringstream ss;
    ss << selectAll;
    int num = 0;
    if (use_mpg == 1){
        ss <<"( MPG BETWEEN " << min_mpg << " AND " << max_mpg << " ) ";
        num ++;
    }
    if (use_ppg == 1 && num != 0){
        ss << " AND " << "(PPG BETWEEN " << min_ppg << " AND " << max_ppg << " ) ";
        num ++;
    }
    if (use_ppg == 1 && num == 0){
        ss << "(PPG BETWEEN " << min_ppg << " AND " << max_ppg << " ) ";
        num ++;
    }
    if (use_rpg == 1 && num != 0){
        ss << " AND " << "(RPG BETWEEN " << min_rpg << " AND " << max_rpg << " ) ";
        num ++;
    } 
    if (use_rpg == 1 && num == 0){
        ss << "(RPG BETWEEN " << min_rpg << " AND " << max_rpg << " ) ";
        num ++;
    }
    if (use_apg == 1 && num != 0){
        ss << " AND " <<  "(APG BETWEEN " << min_apg << " AND " << max_apg << " ) ";      
        num ++; 
    }
    if (use_apg == 1 && num == 0){
        ss << "(APG BETWEEN " << min_apg << " AND " << max_apg << " ) ";  
        num ++;     
    }
    if (use_spg == 1 && num != 0){
        ss <<  " AND " << "(SPG BETWEEN " << min_spg << " AND " << max_spg << " ) ";
        num ++;
    }
    if (use_spg == 1 && num == 0){
        ss << "(SPG BETWEEN " << min_spg << " AND " << max_spg << " ) ";
        num ++;
    }
    if (use_bpg == 1 && num != 0){
        ss <<  " AND " << "(BPG BETWEEN " << min_bpg << " AND " << max_bpg << " ) ";
        num ++;
    }
    if (use_bpg == 1 && num == 0){
        ss << "(BPG BETWEEN " << min_bpg << " AND " << max_bpg << " ) ";
        num ++;
    }
    ss << ";";

    /* execute query */
    result R(W.exec(ss.str()));
    /* print result */
    cout << "PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG" << endl;
    for (result::const_iterator q = R.begin(); q != R.end(); ++q){
        cout << q[0].as<int>() << " " << q[1].as<int>() << " " << q[2].as<int>() << " "
        << q[3].as<string>() << " " << q[4].as<string>() << " " << q[5].as<int>() << " "
        << q[6].as<int>() << " " << q[7].as<int>() << " " << q[8].as<int>() << " "
        << fixed << setprecision(1)
        << q[9].as<double>() << " " << q[10].as<double>() << endl;
    }
    W.commit();
}


void query2(connection *C, string team_color)
{
    work W(*C);
    /* query */
    stringstream ss;
    ss << " SELECT TEAM.NAME FROM TEAM, COLOR WHERE " 
    << "TEAM.COLOR_ID = COLOR.COLOR_ID AND "
    << "COLOR.NAME = " << W.quote(team_color) << ";";
    /* execute */
    result R(W.exec(ss.str()));
    /* print */
    cout << "NAME" <<endl;
    for (result::const_iterator q = R.begin(); q != R.end(); ++q){
        cout << q[0].as<string>() <<endl;
    } 
    W.commit();
}


void query3(connection *C, string team_name)
{
    work W(*C);
    /* query */
    stringstream ss;
    ss << "SELECT FIRST_NAME, LAST_NAME FROM PLAYER, TEAM WHERE PLAYER.TEAM_ID = TEAM.TEAM_ID AND TEAM.NAME = "
    << W.quote(team_name) << " ORDER BY PPG DESC;";
    /* execute */
    result R(W.exec(ss.str()));
    /* print */
    cout << "FIRST_NAME " << "LAST_NAME" << endl;
    for (result::const_iterator q = R.begin(); q != R.end(); ++q){
        cout << q[0].as<string>() << " " << q[1].as<string>() << endl;
    }
    W.commit();
}


void query4(connection *C, string team_state, string team_color)
{
    work W(*C);
    /* query */
    stringstream ss;
    ss << "SELECT FIRST_NAME, LAST_NAME, UNIFORM_NUM FROM PLAYER, TEAM, STATE, COLOR WHERE PLAYER.TEAM_ID = TEAM.TEAM_ID AND TEAM.STATE_ID = STATE.STATE_ID AND TEAM.COLOR_ID = COLOR.COlOR_ID AND STATE.NAME = "
    << W.quote(team_state) << " AND COLOR.NAME = " << W.quote(team_color) << ";";
    /* execute */
    result R(W.exec(ss.str()));
    /* print */
    cout << "FIRST_NAME " << "LAST_NAME " << "UNIFORM_NUM" << endl;
    for (result::const_iterator q = R.begin(); q != R.end(); ++q){
        cout << q[0].as<string>() << " " << q[1].as<string>() << " " << q[2].as<int>()<< endl;
    }
    W.commit();
}


void query5(connection *C, int num_wins)
{
    work W(*C);
    /* query */
    stringstream ss;
    ss << "SELECT FIRST_NAME, LAST_NAME, TEAM.NAME, TEAM.WINS FROM PLAYER, TEAM WHERE PLAYER.TEAM_ID = TEAM.TEAM_ID AND TEAM.WINS > "
    << num_wins << ";";
    /* execute */
    result R(W.exec(ss.str()));
    /* print */
    cout << "FIRST_NAME " << "LAST_NAME " << "NAME " << "WINS" << endl;
    for (result::const_iterator q = R.begin(); q != R.end(); ++q){
        cout << q[0].as<string>() << " " << q[1].as<string>() << " " << q[2].as<string>() << " " << q[3].as<int>() << endl;
    }
    W.commit();
}
