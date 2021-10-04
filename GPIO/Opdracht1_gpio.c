#include "PJ_RPI.h"
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

int main()
{
	//MSQL
	MYSQL *con = mysql_init(NULL);

	//Variables
	time_t current_time;
    char* c_time_string;
	char state1[20]= "GPIO17 is high";
	char state2[20]= "GPIO17 is low";
	char insert[255];
	struct bcm2835_peripheral gpio = {GPIO_BASE};

	if (con == NULL)
	{
		fprintf(stderr, "%s\n", mysql_error(con));
		exit(1);
	}

	if (mysql_real_connect(con, "localhost", "webuser", "secretpassword", "Opdracht1_gpio", 0, NULL, 0) == NULL)
	{
		fprintf(stderr, "%s\n", mysql_error(con));
		mysql_close(con);
		exit(1);
	}

	if (mysql_query(con, "DROP TABLE IF EXISTS gpio")) {
		finish_with_error(con);
	}

	if (mysql_query(con, "CREATE TABLE gpio (id INT PRIMARY KEY AUTO_INCREMENT, State VARCHAR(255),DateTime VARCHAR(255))")) {
		finish_with_error(con);
	}

	if(map_peripheral(&gpio) == -1) 
	{
		printf("Failed to map the physical GPIO registers into the virtual memory space.\n");
		return -1;
	}

	// Define gpio 17 as output
	INP_GPIO(17);
	OUT_GPIO(17);           //BCM 17 || wPi 0 || Physical 11

	while(1)
	{
		
        for(int i=1;i<10;i++)
        {
            // Toggle 17 (blink a led!)
            GPIO_SET = 1 << 17;
			current_time = time(NULL);
			c_time_string = ctime(&current_time);
			sprintf(insert,"INSERT INTO gpio(id, State, DateTime) VALUES(%d,'%s','%s')",i,state1,c_time_string);
			if (mysql_query(con,insert)) {
				finish_with_error(con);
			}
            sleep(1);

            i++;

            GPIO_CLR = 1 << 17;
			current_time = time(NULL);
			c_time_string = ctime(&current_time);
			sprintf(insert,"INSERT INTO gpio(id, State, DateTime) VALUES(%d,'%s','%s')",i,state2,c_time_string);
			if (mysql_query(con,insert)) {
				finish_with_error(con);
			}
            sleep(1);
        }

		//show table
		if (mysql_query(con, "SELECT * FROM gpio"))
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

	return 0;	

}
