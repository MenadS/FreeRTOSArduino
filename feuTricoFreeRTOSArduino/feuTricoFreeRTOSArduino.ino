#include <Arduino_FreeRTOS.h>
#include <semphr.h> 

#define RED_LED_PIN 10
#define YELLOW_LED_PIN 9
#define GREEN_LED_PIN 8
#define BLUE_LED_PIN 7

SemaphoreHandle_t xRedBlueSemaphore, xYellowSemaphore, xGreenSemaphore;


TaskHandle_t RedBlueTask_Handle, YellowTask_Handle, GreenTask_Handle;

void RedBlueTask(void *pvParameters) {
  (void) pvParameters;

  pinMode(BLUE_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);

  for(;;)
  {
    // Pour prendre un sémaphore
    if (xSemaphoreTake(xRedBlueSemaphore, portMAX_DELAY) == pdTRUE) 
    {
        digitalWrite(RED_LED_PIN, HIGH); // Allume la LED rouge
        digitalWrite(BLUE_LED_PIN, HIGH); // Allume la LED bleue
        vTaskDelay(10000 / portTICK_PERIOD_MS); // Allume pendant 10 secondes
        
        digitalWrite(RED_LED_PIN, LOW); // Eteint la LED rouge
        digitalWrite(BLUE_LED_PIN, LOW); // Eteint la LED bleue
        
        xSemaphoreGive(xYellowSemaphore); // Libérer le sémaphore après utilisation
    } else {
        digitalWrite(RED_LED_PIN, HIGH); // Allume la LED rouge
        digitalWrite(BLUE_LED_PIN, LOW); // Allume la LED bleue
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Allume pendant 10 secondes
    }
  }
  
}

void YellowTask(void *pvParameters) {
  (void) pvParameters;
  
  pinMode(YELLOW_LED_PIN, OUTPUT);

  for(;;)
  {
    if(xSemaphoreTake(xYellowSemaphore, portMAX_DELAY) == pdTRUE)
    {
        for (int i = 0; i < 2; i++) 
        {
          digitalWrite(YELLOW_LED_PIN, HIGH); // Allume la LED jaune pendant 2 secondes
          vTaskDelay(1000 / portTICK_PERIOD_MS);
          digitalWrite(YELLOW_LED_PIN, LOW); // Eteint la LED jaune
          vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
        
        digitalWrite(YELLOW_LED_PIN, HIGH); // Allume la LED jaune pendant 3 secondes
        vTaskDelay(3000 / portTICK_PERIOD_MS);
        digitalWrite(YELLOW_LED_PIN, LOW); // Eteint la LED jaune
        
      xSemaphoreGive(xGreenSemaphore); // Libérer le sémaphore après utilisation
      
    } else {
        digitalWrite(RED_LED_PIN, HIGH); // Allume la LED rouge
        digitalWrite(BLUE_LED_PIN, LOW); // Allume la LED bleue
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Allume pendant 10 secondes
    }
  }
  
}

void GreenTask(void *pvParameters) {
  (void) pvParameters;

  pinMode(GREEN_LED_PIN, OUTPUT);

  for(;;)
  {
    if(xSemaphoreTake(xGreenSemaphore, portMAX_DELAY) == pdTRUE)
    {
        digitalWrite(GREEN_LED_PIN, HIGH); // Allume la LED verte pendant 10 secondes
        vTaskDelay(10000 / portTICK_PERIOD_MS);
        digitalWrite(GREEN_LED_PIN, LOW); // Eteint la LED verte
        
          digitalWrite(RED_LED_PIN, HIGH); // Allume la LED rouge
          digitalWrite(BLUE_LED_PIN, HIGH); // Allume la LED bleue
          vTaskDelay(1000 / portTICK_PERIOD_MS); // Allume pendant 10 secondes
  
      xSemaphoreGive(xRedBlueSemaphore); // Libérer le sémaphore après utilisation
      
    } else {
        digitalWrite(RED_LED_PIN, HIGH); // Allume la LED rouge
        digitalWrite(BLUE_LED_PIN, LOW); // Allume la LED bleue
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Allume pendant 10 secondes
    }
  }
 
}


void setup() {

    xRedBlueSemaphore = xSemaphoreCreateBinary();
    xYellowSemaphore = xSemaphoreCreateBinary();
    xGreenSemaphore = xSemaphoreCreateBinary();
    
    xSemaphoreGive(xRedBlueSemaphore);
    
    xTaskCreate(RedBlueTask, "RedBlueTask", 128, NULL, 1, &RedBlueTask_Handle);
    xTaskCreate(YellowTask, "YellowTask", 128, NULL, 1, &YellowTask_Handle);
    xTaskCreate(GreenTask, "GreenTask", 128, NULL, 1, &GreenTask_Handle);
    vTaskStartScheduler();
}
void loop() {
    
}
