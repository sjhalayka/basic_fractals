#include <iostream>
#include <vector>
#include <cmath>
using namespace std;


class quaternion
{
public:
    inline quaternion(void) : x(0.0f), y(0.0f), z(0.0f), w(0.0f) { /*default constructor*/ }
    inline quaternion(const float src_x, const float src_y, const float src_z, const float src_w) : x(src_x), y(src_y), z(src_z), w(src_w) { /* custom constructor */ }
    
    inline float self_dot(void) const
    {
        return x*x + y*y + z*z + w*w;
    }
    
    inline float magnitude(void) const
    {
        return sqrtf(self_dot());
    }
    
    quaternion operator*(const quaternion &right) const
    {
        quaternion ret;
        
        ret.x = x*right.x - y*right.y - z*right.z - w*right.w;
        ret.y = x*right.y + y*right.x + z*right.w - w*right.z;
        ret.z = x*right.z - y*right.w + z*right.x + w*right.y;
        ret.w = x*right.w + y*right.z - z*right.y + w*right.x;
        
        return ret;
    }
    
    quaternion operator+(const quaternion &right) const
    {
        quaternion ret;
        
        ret.x = x + right.x;
        ret.y = y + right.y;
        ret.z = z + right.z;
        ret.w = w + right.w;
        
        return ret;
    }

    quaternion operator-(const quaternion &right) const
    {
        quaternion ret;
        
        ret.x = x - right.x;
        ret.y = y - right.y;
        ret.z = z - right.z;
        ret.w = w - right.w;
        
        return ret;
    }

    
    float x, y, z, w;
};


void get_trajectory_properties(
                               const vector<quaternion> &points,
                               float &length,
                               float &displacement,
                               float &magnitude)
{
    if (points.size() == 0)
    {
        length = displacement = magnitude = 0.0f;
        return;
    }
    else if (points.size() == 1)
    {
        length = displacement = 0.0f;
        magnitude = points[0].magnitude();
        return;
    }
    
    length = 0.0f;
    
    for (size_t i = 0; i < points.size() - 1; i++)
        length += (points[i + 1] - points[i]).magnitude();
    
    displacement = (points[points.size() - 1] - points[0]).magnitude();
    magnitude = points[points.size() - 1].magnitude();
}



float iterate_4d_length(vector< quaternion > &trajectory_points,
                        quaternion Z,
                        const quaternion C,
                        const short unsigned int max_iterations,
                        const float threshold)
{
    trajectory_points.clear();
    trajectory_points.push_back(Z);
    
    float length = 0.0f;
    float displacement = 0.0f;
    float magnitude = 0.0f;
    
    for (short unsigned int i = 0; i < max_iterations; i++)
    {
        Z = Z*Z + C;
        
        trajectory_points.push_back(Z);
        
        get_trajectory_properties(trajectory_points,
                                  length,
                                  displacement,
                                  magnitude);
        
        if (length >= threshold)
            break;
    }
    
    return length;
}

float iterate_4d_displacement(vector< quaternion > &trajectory_points,
                        quaternion Z,
                        const quaternion C,
                        const short unsigned int max_iterations,
                        const float threshold)
{
    trajectory_points.clear();
    trajectory_points.push_back(Z);
    
    float length = 0.0f;
    float displacement = 0.0f;
    float magnitude = 0.0f;
    
    for (short unsigned int i = 0; i < max_iterations; i++)
    {
        Z = Z*Z + C;
        
        trajectory_points.push_back(Z);
        
        get_trajectory_properties(trajectory_points,
                                  length,
                                  displacement,
                                  magnitude);
        
        if (displacement >= threshold)
            break;
    }
    
    return displacement;
}


float iterate_4d_magnitude(vector< quaternion > &trajectory_points,
                           quaternion Z,
                           const quaternion C,
                           const short unsigned int max_iterations,
                           const float threshold)
{
    trajectory_points.clear();
    trajectory_points.push_back(Z);
    
    for (short unsigned int i = 0; i < max_iterations; i++)
    {
        Z = Z*Z + C;
        
        trajectory_points.push_back(Z);
        
        if (Z.magnitude() >= threshold)
            break;
    }
    
    return Z.magnitude();
}




void generate_and_save_histogram(const vector<float> &input,
                                 const char *const file_name)
{
    // Insert OpenCV code here to generate and save a histogram
}


int main(void)
{
    size_t res = 30;
    
    float x_grid_max = 1.5;
    float y_grid_max = 1.5;
    float z_grid_max = 1.5;
    float x_grid_min = -x_grid_max;
    float y_grid_min = -y_grid_max;
    float z_grid_min = -z_grid_max;
    size_t x_res = res;
    size_t y_res = res;
    size_t z_res = res;
    
    float z_w = 0;
    quaternion C;
    C.x = 0.3f;
    C.y = 0.5f;
    C.z = 0.4f;
    C.w = 0.2f;
    unsigned short int max_iterations = 8;
    float threshold = 4.0f;
    
    const float x_step_size = (x_grid_max - x_grid_min) / (x_res - 1);
    const float y_step_size = (y_grid_max - y_grid_min) / (y_res - 1);
    const float z_step_size = (z_grid_max - z_grid_min) / (z_res - 1);
    
    quaternion Z(x_grid_min, y_grid_min, z_grid_min, z_w);
    
    vector<float> lengths;
    vector<float> displacements;
    vector<float> magnitudes;
    
    for (size_t z = 0; z < z_res; z++, Z.z += z_step_size)
    {
        cout << "Z slice " << z + 1 << " of " << z_res << endl;
        
        Z.x = x_grid_min;
        
        for (size_t x = 0; x < x_res; x++, Z.x += x_step_size)
        {
            Z.y = y_grid_min;
            
            for (size_t y = 0; y < y_res; y++, Z.y += y_step_size)
            {
                vector<quaternion> points;
                
                iterate_4d_length(points, Z, C, max_iterations, threshold);
                
                float length = 0.0f;
                float displacement = 0.0f;
                float magnitude = 0.0f;
                
                get_trajectory_properties(points,
                                          length,
                                          displacement,
                                          magnitude);
                
                lengths.push_back(length);
                displacements.push_back(displacement);
                magnitudes.push_back(magnitude);
            }
            
            cout << endl;
        }
        
        cout << endl << endl;
    }
    
    generate_and_save_histogram(lengths, "lengths.png");
    generate_and_save_histogram(displacements, "displacements.png");
    generate_and_save_histogram(magnitudes, "magnitudes.png");
    
    return 0;
}
