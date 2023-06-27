#include "utils.hpp"

IPAddress masked2(IPAddress ip)
{
	IPAddress result = {ip.p1, ip.p2, ip.p3, ip.p4, ip.mask};

	if (stoi(ip.mask) <= 8)
	{
		int intp1 = stoi(ip.p1);
		intp1 >>= (8 - stoi(ip.mask));
		intp1 <<= (8 - stoi(ip.mask));
		result.p1 = to_string(intp1);
		result.p2 = "0";
		result.p3 = "0";
		result.p4 = "0";
	}
	else if (stoi(ip.mask) <= 16)
	{
		int intp2 = stoi(ip.p2);
		intp2 >>= (16 - stoi(ip.mask));
		intp2 <<= (16 - stoi(ip.mask));
		result.p2 = to_string(intp2);
		result.p3 = "0";
		result.p4 = "0";
	}
	else if (stoi(ip.mask) <= 24)
	{
		int intp3 = stoi(ip.p3);
		intp3 >>= (24 - stoi(ip.mask));
		intp3 <<= (24 - stoi(ip.mask));
		result.p3 = to_string(intp3);
		result.p4 = "0";
	}
	else if (stoi(ip.mask) <= 32)
	{
		int intp4 = stoi(ip.p4);
		intp4 >>= (32 - stoi(ip.mask));
		intp4 <<= (32 - stoi(ip.mask));
		result.p4 = to_string(intp4);
	}

	return result;
}

routeRecord masked(IPAddress ip, int dist)
{
	routeRecord result = {ip.p1, ip.p2, ip.p3, ip.p4, ip.mask, dist, "0", "0", "0", "0", "0", true, true};

	if (stoi(ip.mask) <= 8)
	{
		int intp1 = stoi(ip.p1);
		intp1 >>= (8 - stoi(ip.mask));
		intp1 <<= (8 - stoi(ip.mask));
		result.p1 = to_string(intp1);
		result.p2 = "0";
		result.p3 = "0";
		result.p4 = "0";
	}
	else if (stoi(ip.mask) <= 16)
	{
		int intp2 = stoi(ip.p2);
		intp2 >>= (16 - stoi(ip.mask));
		intp2 <<= (16 - stoi(ip.mask));
		result.p2 = to_string(intp2);
		result.p3 = "0";
		result.p4 = "0";
	}
	else if (stoi(ip.mask) <= 24)
	{
		int intp3 = stoi(ip.p3);
		intp3 >>= (24 - stoi(ip.mask));
		intp3 <<= (24 - stoi(ip.mask));
		result.p3 = to_string(intp3);
		result.p4 = "0";
	}
	else if (stoi(ip.mask) <= 32)
	{
		int intp4 = stoi(ip.p4);
		intp4 >>= (32 - stoi(ip.mask));
		intp4 <<= (32 - stoi(ip.mask));
		result.p4 = to_string(intp4);
	}

	return result;
}

IPAddress findBroadcast(IPAddress ip)
{
	if (stoi(ip.mask) <= 8)
	{
		int intp1 = stoi(ip.p1);
		intp1 = intp1 | (1 << (8 - stoi(ip.mask))) - 1;
		ip.brd1 = to_string(intp1);
		ip.brd2 = "255";
		ip.brd3 = "255";
		ip.brd4 = "255";
	}
	else if (stoi(ip.mask) <= 16)
	{
		int intp2 = stoi(ip.p2);
		intp2 = intp2 | (1 << (16 - stoi(ip.mask))) - 1;
		ip.brd1 = ip.p1;
		ip.brd2 = to_string(intp2);
		ip.brd3 = "255";
		ip.brd4 = "255";
	}
	else if (stoi(ip.mask) <= 24)
	{
		int intp3 = stoi(ip.p3);
		intp3 = intp3 | (1 << (24 - stoi(ip.mask))) - 1;
		ip.brd1 = ip.p1;
		ip.brd2 = ip.p2;
		ip.brd3 = to_string(intp3);
		ip.brd4 = "255";
	}
	else if (stoi(ip.mask) <= 32)
	{
		int intp4 = stoi(ip.p4);
		intp4 = intp4 | (1 << (32 - stoi(ip.mask))) - 1;
		ip.brd1 = ip.p1;
		ip.brd2 = ip.p2;
		ip.brd3 = ip.p3;
		ip.brd4 = to_string(intp4);
	}
	return ip;
}

int getTime(chrono::steady_clock::time_point *startTime)
{
	auto endTime = chrono::steady_clock::now();
	auto duration = chrono::duration<double, milli>(endTime - *startTime).count();
	return duration;
}

IPAddress findMask(IPAddress ip, vector<IPAddress> *addrTable)
{
	for (int i = 32; i >= 0; i--)
	{
		ip.mask = to_string(i);
		ip = masked2(ip);

		for (auto curr : (*addrTable))
		{
			if (curr.p1 == ip.p1 && curr.p2 == ip.p2 && curr.p3 == ip.p3 && curr.p4 == ip.p4)
				return ip;
		}
	}
	return ip;
}

void printTable(vector<routeRecord> *routeTable)
{

	for (routeRecord record : (*routeTable))
	{
		cout << record.p1 + "." + record.p2 + "." + record.p3 + "." + record.p4 + "/" + record.mask;
		cout << (record.reachable ? " distance " + to_string(record.distance) : " reachable ");
		cout << (record.direct ? " connected directly " : " via " + record.via1 + "." + record.via2 + "." + record.via3 + "." + record.via4);
		cout << "\n";
	}
	cout << "\n";
}