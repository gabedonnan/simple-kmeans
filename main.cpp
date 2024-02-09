#include <vector>
#include <math.h>
#include <float.h>
#include <random>
#include <iostream>
#include <fstream>
#include <string>


class KMeans {
    public:
        KMeans(uint16_t _k) : k(_k) {
            for (int i = 0; i < k; i++)
                centroids.emplace_back( // Randomly assign centroids
                    static_cast<float>(rand()) / (static_cast<float>(RAND_MAX)/100.0f),
                    static_cast<float>(rand()) / (static_cast<float>(RAND_MAX)/100.0f)
                );
        }

        void read_points_from_file(std::string file_name) {
            std::ifstream file (file_name);
            std::string contents;
            int delim_pos;  // Cannot be size_t as size_t is unsigned and missing .find will cause underflow
            std::string x_pos_string;
            std::string y_pos_string;

            if (file.is_open()) {
                while (file) {
                    // Files of form: x_pos, y_pos\nx_pos, y_pos etc.
                    std::getline(file, contents);
                    delim_pos = contents.find(", ");
                    if (delim_pos != -1) {
                        x_pos_string = contents.substr(0, delim_pos);
                        y_pos_string = contents.substr(delim_pos + 2, contents.length() - 1);
                        // std::cout << x_pos_string << " " << y_pos_string << std::endl;
                        coords.emplace_back(std::stof(x_pos_string), std::stof(y_pos_string));
                    } else {
                        break;
                    }
                }
            }
        }

        void create_random_points(uint64_t num) {
            for (int i = 0; i < num; i++)
                coords.emplace_back( // Randomly assign centroids
                    static_cast<float>(rand()) / (static_cast<float>(RAND_MAX)/100.0f),
                    static_cast<float>(rand()) / (static_cast<float>(RAND_MAX)/100.0f)
                );
        }

        void run(int num_iterations) {
            for (int i = 0; i < num_iterations; i++)
                update_centroids();
        }

        void print_coords() {
            for (auto &coord : coords)
                std::cout << coord.first << " " << coord.second << std::endl;
        }

        void print_centroids() {
            for (auto &centroid : centroids)
                std::cout << centroid.first << " " << centroid.second << std::endl;
        }

    private:
        uint16_t k;
        std::vector<std::pair<float, float>> coords;
        std::vector<std::pair<float, float>> centroids;


        void update_centroids() {
            std::vector<std::vector<std::pair<float, float>>> closest_points(k);  // Closest points to each mean
            float cost = 0.0f;
            float closest = FLT_MAX;
            uint16_t closest_idx;
            float euclidean;

            for (auto &coord : coords) {
                closest = FLT_MAX;
                for (int i = 0; i < k; i++) {
                    euclidean = euclidean_distance(centroids[i], coord);
                    if (euclidean < closest) {
                        closest = euclidean;
                        closest_idx = i;
                    }
                }

                closest_points[closest_idx].push_back(coord);
            }

            float x_sum;
            float y_sum;
            uint16_t idx = 0;

            for (auto &valset : closest_points) {
                if (valset.size() == 0) {
                    idx++;
                    continue;
                }
              
                x_sum = 0.0f;
                y_sum = 0.0f;
                for (auto &coord : valset) {
                    x_sum += coord.first;
                    y_sum += coord.second;
                }

                x_sum /= static_cast<float>(valset.size());
                y_sum /= static_cast<float>(valset.size());
                centroids[idx] = std::make_pair(x_sum, y_sum);
                idx++;
            }
        }

        inline float euclidean_distance(std::pair<float, float>& p1, std::pair<float, float>& p2) {
            return sqrt(pow(p1.first - p2.first, 2) + pow(p1.second - p2.second, 2));
        }
};

int main() {
  KMeans km (3);
  km.create_random_points(10000);
  // km.print_coords();
  km.print_centroids();
  km.run(10);
  km.print_centroids();
  return 0;
}
