#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

// Having trouble with allocating memory for the database struck based on the amount of rows it doesn't start with.

struct Address {
	int id;
	int set;
	char *name;
	char *email;
};

struct Database {
	struct Address *rows;
	//number of database fields
	int num_rows;
};

struct Connection {
	FILE *file;
	struct Database *db;
};

void Database_close (struct Connection *conn)
{
	if(conn){
		if(conn->file) fclose(conn->file);
		if(conn->db) free(conn->db);
		free(conn);
	}
}

void die(struct Connection *conn, const char *message)
{
	if(errno) {
		perror(message);
	}
	else{
		printf("ERROR: %s\n", message);
	}

	Database_close(conn);
	exit(1);
}

void Address_print(struct Address *addr)
{
	printf("%d %s %s\n", addr->id, addr->name, addr->email);
}

void Database_load(struct Connection *conn)
{
	int rc = fread(conn->db, sizeof(struct Database), 1, conn->file);
	if(rc != 1) die(conn, "Failed to load database.");
}

struct Connection *Database_open(const char *filename, char mode)
{
	struct Connection *conn = malloc( sizeof(struct Connection));
	if(!conn) die(conn, "Memory error");

	if (mode == 'c'){
		conn->file = fopen(filename, "w");
	}
	else {
		conn->file = fopen(filename, "r+");

		if (conn->file){
			Database_load(conn);
		}
	}

	if (!conn->file) die(conn, "Failed to open the file");

	return conn;
}

void Database_write (struct Connection *conn)
{
	rewind(conn->file);

	int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
	if(rc != 1) die(conn, "Failed to write to database");

	rc = fflush(conn->file);
	if (rc == -1) die(conn, "Cannot flush database");

}

void Database_create(struct Connection *conn)
{
	int i = 0;
	conn->db->num_rows = 0;
	conn->db->rows = (struct Address*) malloc(conn->db->num_rows * sizeof(struct Address));

}

void Database_set (struct Connection *conn, int id, const char *name, const char *email)
{
	
	int iter = 0;
	struct Address *addr = &conn->db->rows[id+iter];
	while(addr->set){
		struct Address *addr = &conn->db->rows[id+iter];
		iter++;
	}
	//add row to num_rows
	conn->db->num_rows += 1;

	//if (addr->set) die(conn, "Already set, delete it first");

	addr->set = 1;
	//WARNING: bug, read the "How to Breack it", and fix this ---- FIXED
	addr->name[ sizeof(addr->name)- 1] = '\0';
	int MAX_SIZE_NAME = sizeof(name) / sizeof(char);
	char *res = strncpy(addr->name, name, MAX_SIZE_NAME-1);
	
	// demostrate the strncpy bug ---- FIXED
	if(!res) die(conn, "Name copy failed");

	addr->email[ sizeof(addr->email)- 1] = '\0';
	int MAX_SIZE_EMAIL = sizeof(email) / sizeof(char);
	res = strncpy(addr->email, email, MAX_SIZE_EMAIL-1);
	
	if(!res) die(conn, "Email copy failed");

}

void Database_get (struct Connection *conn, int id)
{
	struct Address *addr = &conn->db->rows[id];
	if( addr->set){
		Address_print(addr);
	}
	else{
		die(conn, "ID is not set");
	}
}

void Database_delete (struct Connection *conn, int id)
{
	struct Address addr = {.id = id, .set = 0};
	conn->db->rows[id] = addr;
}

void Database_list(struct Connection *conn)
{
	int i = 0;
	struct Database *db = conn->db;

	for(i = 0; i < db->num_rows; i++){
		struct Address *cur = &db->rows[i];

		if(cur->set){
			Address_print(cur);
		}
	}
}

int main(int argc, char *argv[])
{
	if(argc < 3) {
		printf("USAGE: ex17 <dbfile> <action> [action params]\n");
		return 1;
	}

	char *filename = argv[1];
	char action = argv[2][0];
	struct Connection *conn = Database_open(filename, action);
	int id = 0;

	if(argc > 3) id = atoi(argv[3]);

	switch(action){
		case 'c':
			//Database_create(conn);
			//Database_write(conn);
			break;

		case 'g':
			if(argc !=4) die(conn, "Need an id to get");

			Database_get(conn, id);
			break;

		case 's':
			if(argc !=6) die(conn, "Need an id, name, and email to set");

			Database_set(conn, id, argv[4], argv[5]);
			Database_write(conn);
			break;

		case 'd':
			if(argc !=4) die(conn, "Need an id to delete");

			Database_delete(conn, id);
			Database_write(conn);
			break;

		case 'l':
			Database_list(conn);
			break;
		default:
			die(conn, "Invalid action, only: c=create, g=get, s=set, d=delete, l=list");
	}

	Database_close(conn);

	return 0;
}