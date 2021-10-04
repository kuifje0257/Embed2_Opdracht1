#include <wiringPi.h>
#include <stdio.h>
#include <mysql.h>
#include <stdlib.h>
#include <time.h>

void finish_with_error(MYSQL *con)
{
fprintf(stderr, "%s\n", mysql_error(con));
mysql_close(con);
exit(1);
}

int main (void)
{
  //MSQL
	MYSQL *con = mysql_init(NULL);
	time_t current_time;
  char* c_time_string;
	char state1[20]= "BCM17 is high";
	char state2[20]= "BCM17 is low";
	char insert[255];

	if (con == NULL)
	{
		fprintf(stderr, "%s\n", mysql_error(con));
		exit(1);
	}

	if (mysql_real_connect(con, "localhost", "webuser", "secretpassword", "Opdracht1_wiringpi", 0, NULL, 0) == NULL)
	{
		fprintf(stderr, "%s\n", mysql_error(con));
		mysql_close(con);
		exit(1);
	}

	if (mysql_query(con, "DROP TABLE IF EXISTS wiringpi")) {
		finish_with_error(con);
	}

	if (mysql_query(con, "CREATE TABLE wiringpi (id INT PRIMARY KEY AUTO_INCREMENT, State VARCHAR(255),DateTime VARCHAR(255))")) {
		finish_with_error(con);
	}

  wiringPiSetup () ;
  pinMode (0, OUTPUT) ; //BCM 17 || wPi 0 || Physical 11
  for (;;)
  {

    for(int i=1;i<10;i++)
    {
      digitalWrite (0, HIGH) ; 
      current_time = time(NULL);
      c_time_string = ctime(&current_time);
      sprintf(insert,"INSERT INTO wiringpi(id, State, DateTime) VALUES(%d,'%s','%s')",i,state1,c_time_string);
      if (mysql_query(con,insert)) {
        finish_with_error(con);
      }
      delay (1000) ;

      i++;

      digitalWrite (0,  LOW) ; 
			current_time = time(NULL);
			c_time_string = ctime(&current_time);
			sprintf(insert,"INSERT INTO wiringpi(id, State, DateTime) VALUES(%d,'%s','%s')",i,state2,c_time_string);
			if (mysql_query(con,insert)) {
				finish_with_error(con);
			}
        delay (1000) ;
    }

    //show table
    if (mysql_query(con, "SELECT * FROM wiringpi"))
    {
        finish_with_error(con);
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL)
    {
        finish_with_error(con);
    }

    int num_fields = mysql_num_fields(result);

    MYSQL_ROW row;

    while ((row = mysql_fetch_row(result)))
    {
        for(int i = 0; i < num_fields; i++)
        {
            printf("%s ", row[i] ? row[i] : "NULL");
        }

        printf("\n");
    }

    mysql_free_result(result);
    mysql_close(con);

    exit(0);
  }
  return 0 ;
}