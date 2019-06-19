#include <iostream>
#include <cmath>
#include <vector>
using namespace std;


float iterate_1d(vector<float> &trajectory_points,
                 float Z,
                 const float C,
                 const short unsigned int max_iterations,
                 const float threshold)
{
    trajectory_points.clear();
    trajectory_points.push_back(Z);

    for (short unsigned int i = 0; i < max_iterations; i++)
    {
        Z = Z*Z + C;
        
        trajectory_points.push_back(Z);
        
        if (fabsf(Z) >= threshold)
            break;
    }
    
    return fabsf(Z);
}

int main(void)
{
    const float x_grid_max = 1.5;
    const float x_grid_min = -x_grid_max;
    const size_t x_res = 30;
    const float x_step_size = (x_grid_max - x_grid_min) / (x_res - 1);

    const float C = 0.2f;
    const unsigned short int max_iterations = 8;
    const float threshold = 4.0f;
    
    float Z = x_grid_min;

    vector<float> trajectory_points;
    
    for (size_t x = 0; x < x_res; x++, Z += x_step_size)
    {
        float magnitude = iterate_1d(trajectory_points, Z, C, max_iterations, threshold);
        
        if(magnitude < threshold)
            cout << '*';
        else
            cout << '.';
    }
    
    cout << endl;
    
    return 0;
}



