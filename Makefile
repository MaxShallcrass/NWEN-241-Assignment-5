dbserver: server.o dbms.o
	g++ server.o dbms.o -o dbserver

server.o: server.hh server.cc
	g++ -c server.cc

dbms.o: dbms.cc dbms.hh
	g++ -c dbms.cc

client: client.o
	g++ -o client client.o

client.o: client.cc server.hh
	g++ -c client.cc

clean:
	rm -rf *.o dbserver client
