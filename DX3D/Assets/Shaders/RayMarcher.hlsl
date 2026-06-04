struct PS_INPUT
{
    float4 position : SV_Position;
    float4 color : COLOR0;
};

//Distance Function for the sphere
//Shoots a ray from the camera to the defined position of the sphere then calculates the distance between them 
//and subtracts the radius of the sphere to determine if the ray has hit the surface of the sphere or not. 
//It returns a positive value if the ray is outside the sphere, zero if it is on the surface, and a negative value if it is inside the sphere.
float SphereDistance(float3 samplePoint, float3 sphereCenter, float radius)
{
    return length(samplePoint - sphereCenter) - radius;
}

float4 PSMain(PS_INPUT input) : SV_TARGET
{
    //Get UV Coordinates from the input color which is passed from the vertex shader
    float2 uv = input.color.xy;
    
    //Aspect Ratio Stretching Fix
    //The reason for the value 1.777f is because aspect ratio of the screen is 16:9
    uv.x *= 1.777f; 
    
    //Camera Setup
    float3 rayOrigin = float3(0.0f, 0.0f, -3.0f); 
    float3 rayDirection = normalize(float3(uv, 1.0f)); 

    //Sphere Setup
    float3 sphereCenter = float3(0.0f, 1.15f, 0.0f); //User defined position of the sphere in the screen space
    float sphereRadius = 1.0f; //The size of the sphere

    float3 currentPosition = rayOrigin;

    //Ray Marching Loop
    for (int i = 0; i < 64; i++)
    {
        //Calls the distance function to calculate the distance from the current position of the ray to the surface of the sphere
        float distanceToSphere = SphereDistance(currentPosition, sphereCenter, sphereRadius); 

        //Render the hit Surface by checking if the distance to the sphere is less than to the threshold value
        //Also adding a simple lighting effect by calculating the normal and light direction to give the sphere a more 3D appearance.
        if (distanceToSphere < 0.001f)
        {
            float3 normal = normalize(currentPosition - sphereCenter);
            float3 lightDir = normalize(float3(1.0f, 1.0f, -1.0f));
            float lighting = max(dot(normal, lightDir), 0.2f); 

            return float4(float3(0.89, 0.44, 0.47) * lighting, 1.0f); //change the detection hit to the coolor of the sphere.
        }
        
        currentPosition += rayDirection * distanceToSphere;
    }

    //If there is a detection miss we return the background color
    return float4(0.22f, 0.73f, 0.73f, 1.0f);
}

