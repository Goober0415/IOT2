/* 
 * Project l04_06_sorting
 * Author: Jamie Gavina
 * Date: 2/5/2025
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

#include "Particle.h"
#include <neopixel.h>

SYSTEM_MODE(SEMI_AUTOMATIC);

const int PIXELCOUNT = 46;

Adafruit_NeoPixel pixel(PIXELCOUNT , SPI1 , WS2812B);

void selectionSort(int* arr);
void bubbleSort(int* arr);
void displayArray(int* arr);

void setup() {
  Serial.begin(9600);
  pixel.begin();
  pixel.setBrightness(10);
  randomSeed(analogRead(0));  // Initialize random seed
  
  // Generate initial random array
  int values[PIXELCOUNT];
  for(int i = 0; i < PIXELCOUNT; i++) {
    values[i] = random(255);  // Values from 0 to 254
  }
}

// Convert value to color (blue to red gradient)
uint32_t valueToColor(int value) {
  if(value < 128) {
    return pixel.Color(0, 0, map(value, 0, 127, 0, 255));
  } else {
    return pixel.Color(map(value, 128, 255, 0, 255), 0, 0);
  }
}

// Display array values as colors
void displayArray(int* arr) {
  for(int i = 0; i < PIXELCOUNT; i++) {
    pixel.setPixelColor(i, valueToColor(arr[i]));
  }
  pixel.show();
}

// Bubble sort implementation with visualization
void bubbleSort(int* arr) {
  int n = PIXELCOUNT;
  bool swapped;
  
  for(int i = 0; i < n-1; i++) {
    swapped = false;
    for(int j = 0; j < n-i-1; j++) {
      if(arr[j] > arr[j+1]) {
        // Swap elements
        int temp = arr[j];
        arr[j] = arr[j+1];
        arr[j+1] = temp;
        
        // Visual feedback
        displayArray(arr);
        delay(50);  // Adjust speed as needed
        
        swapped = true;
      }
    }
    
    // If no two elements were swapped in inner loop, array is sorted
    if(!swapped) break;
  }
}

// Selection sort implementation with visualization
void selectionSort(int* arr) {
  int n = PIXELCOUNT;
  
  for(int i = 0; i < n-1; i++) {
    int min_idx = i;
    
    // Find minimum element in unsorted part
    for(int j = i+1; j < n; j++) {
      if(arr[j] < arr[min_idx]) {
        min_idx = j;
      }
    }
    
    // Swap found minimum element with first element of unsorted array
    if(min_idx != i) {
      int temp = arr[i];
      arr[i] = arr[min_idx];
      arr[min_idx] = temp;
      
      // Visual feedback
      displayArray(arr);
      delay(50);  // Adjust speed as needed
    }
  }
}

void loop() {
  static bool useBubbleSort = true;
  
  // Generate new random array
  int values[PIXELCOUNT];
  for(int i = 0; i < PIXELCOUNT; i++) {
    values[i] = random(255);
  }
  
  Serial.println(useBubbleSort ? "Starting Bubble Sort..." : "Starting Selection Sort...");
  displayArray(values);
  delay(2000);
  
  if(useBubbleSort) {
    bubbleSort(values);
  } else {
    selectionSort(values);
  }
  
  // Toggle sort algorithm for next iteration
  useBubbleSort = !useBubbleSort;
  
  delay(5000);  // Wait before generating new random array
}