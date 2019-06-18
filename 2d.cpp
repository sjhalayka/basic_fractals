#include <iostream>
#include <cmath>
#include <vector>
#include <complex>
using namespace std;


float iterate_2d(vector< complex<float> > &trajectory_points,
                 complex<float> Z,
                 const complex<float> C,
                 const short unsigned int max_iterations,
                 const float threshold)
{
    trajectory_points.clear();
    trajectory_points.push_back(Z);

    for (short unsigned int i = 0; i < max_iterations; i++)
    {
        Z = Z*Z + C;
        
        trajectory_points.push_back(Z);
        
        if (abs(Z) >= threshold)
            break;
    }
    
    return abs(Z);
}

int main(void)
{
    const float x_grid_max = 1.5;
    const float x_grid_min = -x_grid_max;
    const size_t x_res = 30;
    const float x_step_size = (x_grid_max - x_grid_min) / (x_res - 1);

    const float y_grid_max = 1.5;
    const float y_grid_min = -y_grid_max;
    const size_t y_res = 30;
    const float y_step_size = (y_grid_max - y_grid_min) / (y_res - 1);

    const complex<float> C(0.2f, 0.5f);
    const unsigned short int max_iterations = 8;
    const float threshold = 4.0f;
    
    complex<float> Z(x_grid_min, y_grid_min);

    vector< complex<float> > trajectory_points;
    
    for (size_t x = 0; x < x_res; x++)
    {
        Z = complex<float>(Z.real(), y_grid_min);
        
        for (size_t y = 0; y < y_res; y++)
        {
            float magnitude = iterate_2d(trajectory_points, Z, C, max_iterations, threshold);
            
            if(magnitude < threshold)
                cout << '*';
            else
                cout << '.';
        
            Z = complex<float>(Z.real(), Z.imag() + y_step_size);
        }
        
        Z = complex<float>(Z.real() + x_step_size, Z.imag());
        
        cout << endl;
    }
    
    cout << endl;
    
    return 0;
}



