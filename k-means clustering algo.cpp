#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <algorithm>
#include <stdio.h>

using namespace std;

class Point {
private:
	int id_point, id_cluster;
	vector<double> values;
	int total_values;

public:
	Point(int id_point, vector<double>& values)
	{
		this->id_point = id_point;
		total_values = values.size();

		for(int i = 0; i < total_values; i++)
			this->values.push_back(values[i]);

		id_cluster = -1;
	}

	int getID()
	{
		return id_point;
	}

	void setCluster(int id_cluster)
	{
		this->id_cluster = id_cluster;
	}

	int getCluster()
	{
		return id_cluster;
	}

	double getValue(int index)
	{
		return values[index];
	}

	int getTotalValues()
	{
		return total_values;
	}

	void addValue(double value)
	{
		values.push_back(value);
	}

};

class Cluster {
private:
	int id_cluster;
	vector<double> central_values;
	vector<Point> points;

public:
	Cluster(int id_cluster, Point point)
	{
		this->id_cluster = id_cluster;

		int total_values = point.getTotalValues();

		for(int i = 0; i < total_values; i++)
			central_values.push_back(point.getValue(i));

		points.push_back(point);
	}

	void addPoint(Point point)
	{
		points.push_back(point);
	}

	bool removePoint(int id_point)
	{
		int total_points = points.size();

		for(int i = 0; i < total_points; i++)
		{
			if(points[i].getID() == id_point)
			{
				points.erase(points.begin() + i);
				return true;
			}
		}
		return false;
	}

	double getCentralValue(int index)
	{
		return central_values[index];
	}

	void setCentralValue(int index, double value)
	{
		central_values[index] = value;
	}

	Point getPoint(int index)
	{
		return points[index];
	}

	int getTotalPoints()
	{
		return points.size();
	}

	int getID()
	{
		return id_cluster;
	}
};

class KMeans {

private:
	int K; // number of clusters
	int total_values, total_points, max_iterations;

    vector<Cluster> clusters;
	vector<Cluster> clusters_final;

	// return ID of nearest center (uses euclidean distance)
	int getIDNearestCenter(Point point)
	{
		double sum = 0.0, min_dist;
		int id_cluster_center = 0;

		for(int i = 0; i < total_values; i++)
		{
			sum += pow(clusters[0].getCentralValue(i) -
					   point.getValue(i), 2.0);
		}

		min_dist = sqrt(sum);

		for(int i = 1; i < K; i++)
		{
			double dist;
			sum = 0.0;

			for(int j = 0; j < total_values; j++)
			{
				sum += pow(clusters[i].getCentralValue(j) -
						   point.getValue(j), 2.0);
			}

			dist = sqrt(sum);

			if(dist < min_dist)
			{
				min_dist = dist;
				id_cluster_center = i;
			}
		}

		return id_cluster_center;
	}

public:
	KMeans(int K, int total_points, int total_values, int max_iterations)
	{
		this->K = K;
		this->total_points = total_points;
		this->total_values = total_values;
		this->max_iterations = max_iterations;
	}

	void run(vector<Point> & points)
	{
	    double min_cost_fuction = 1000000.0;


	    for ( int try100pairs = 0 ; try100pairs < 100 ; try100pairs++ ){

		if(K > total_points)
			return;

		vector<int> prohibited_indexes;

		// choose K distinct values for the centers of the clusters
		for(int i = 0; i < K; i++)
		{
			while(true)
			{
				int index_point = rand() % total_points;

				if(find(prohibited_indexes.begin(), prohibited_indexes.end(),index_point) == prohibited_indexes.end())
				{
					prohibited_indexes.push_back(index_point);
					points[index_point].setCluster(i);
					Cluster cluster(i, points[index_point]);
					clusters.push_back(cluster);
					break;
				}
			}
		}

		int iter = 1;

		while(true)
		{
			bool done = true;

			// associates each point to the nearest center
			for(int i = 0; i < total_points; i++)
			{
				int id_old_cluster = points[i].getCluster();
				int id_nearest_center = getIDNearestCenter(points[i]);

				if(id_old_cluster != id_nearest_center)
				{
					if(id_old_cluster != -1)
						clusters[id_old_cluster].removePoint(points[i].getID());

					points[i].setCluster(id_nearest_center);
					clusters[id_nearest_center].addPoint(points[i]);
					done = false;
				}
			}

			// recalculating the center of each cluster
			for(int i = 0; i < K; i++)
			{
				for(int j = 0; j < total_values; j++)
				{
					int total_points_cluster = clusters[i].getTotalPoints();
					double sum = 0.0;

					if(total_points_cluster > 0)
					{
						for(int p = 0; p < total_points_cluster; p++)
							sum += clusters[i].getPoint(p).getValue(j);
						clusters[i].setCentralValue(j, sum / total_points_cluster);
					}
				}
			}

			if(done == true || iter >= max_iterations)
			{
				break;
			}

			iter++;
		}

		double sum = 0.0;


		for ( int i=0 ; i< K ;i++ ){

            int total_points_cluster = clusters[i].getTotalPoints();

            if(total_points_cluster > 0)
					{
						for(int p = 0; p < total_points_cluster; p++){

                            for(int j = 0; j < total_values; j++)
                                {
                                    sum += pow(clusters[i].getCentralValue(j) - clusters[i].getPoint(p).getValue(j), 2.0);
                                }

						}

					}
                }

                if ( sum < min_cost_fuction ){
                    min_cost_fuction = sum;
                    clusters_final = clusters;
                }

                clusters.clear();
                prohibited_indexes.clear();

                for ( int i = 0 ; i<total_points ; i++ )
                    points[i].setCluster(-1);

	    }

		// shows elements of clusters

		freopen ("clusterswithpoints.txt","w",stdout);
		for(int i = 0; i < K; i++)
		{
			int total_points_cluster =  clusters_final[i].getTotalPoints();

			cout << "Cluster " << clusters_final[i].getID() + 1 << endl;
			for(int j = 0; j < total_points_cluster; j++)
			{
				cout << "Point " << clusters_final[i].getPoint(j).getID() + 1 << ": ";
				for(int p = 0; p < total_values; p++)
					cout << clusters_final[i].getPoint(j).getValue(p) << " ";

				cout << endl;
			}

			cout << "Cluster values: ";

			for(int j = 0; j < total_values; j++)
				cout << clusters_final[i].getCentralValue(j) << " ";

			cout << "\n\n";

		}
		fclose (stdout);

		freopen ("clusters.txt","w",stdout);
		for(int i = 0; i < K; i++)
		{


			for(int j = 0; j < total_values; j++)
				cout << clusters_final[i].getCentralValue(j) << " ";

			cout << " " << endl;

		}
		fclose (stdout);
	}


};

vector<double> split(string str, char delimiter) {

    vector<double> internal;
    stringstream ss(str); // Turn the string into a stream.
    string temp;
    double tempnum;

  while(getline(ss, temp, delimiter)) {

        tempnum = atof(temp.c_str());
        internal.push_back(tempnum);
  }

  return internal;
}

int main ( int argc , char** argv ) {

    srand (time(NULL));

    string line;
    ifstream myfile ("intern.txt");
    int total_points, total_values, K;

    stringstream convert ( argv[1] );
    convert >> K;

    vector<Point> points;
    int i=0;

    if (myfile.is_open())
        {
            while ( getline (myfile,line) )
            {

                vector<double> values;

                vector<double> sep = split(line, ' ');
                total_values = sep.size();

                for(int j = 0; j < total_values; j++)
                    {
                        values.push_back(sep[j]);
                    }

                    Point p(i, values);
                    i++;
                    points.push_back(p);

            }
            total_points = i;
            myfile.close();
        }

    else cout << "Unable to open file";

    KMeans kmeans( K, total_points, total_values, 100);
    kmeans.run(points);

    return 0;
}
