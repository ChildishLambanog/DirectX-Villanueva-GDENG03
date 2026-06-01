struct PS_INPUT
{
    float4 position : SV_Position;
    float4 color : COLOR0;
};

// Distance Function for a clean Sphere
float SphereDistance(float3 samplePoint, float3 sphereCenter, float radius)
{
    return length(samplePoint - sphereCenter) - radius;
}

float4 PSMain(PS_INPUT input) : SV_TARGET
{
    // 1. Take the raw (-1.0 to 1.0) UVs passed directly from C++!
    float2 uv = input.color.xy;
    
    // Fix Aspect Ratio Stretching (Uncomment the line below if your sphere looks like an oval)
    uv.x *= 1.777f; 
    
    // 2. Camera Setup
    float3 rayOrigin = float3(0.0f, 0.0f, -3.0f);
    float3 rayDirection = normalize(float3(uv, 1.0f));

    // 3. Perfect Center Sphere (0,0)
    float3 sphereCenter = float3(0.0f, 1.15f, 0.0f);
    float sphereRadius = 1.0f;

    float3 currentPosition = rayOrigin;

    // 4. Ray Marching Loop
    for (int i = 0; i < 64; i++)
    {
        float distanceToSphere = SphereDistance(currentPosition, sphereCenter, sphereRadius);

        // Render the hit Surface
        if (distanceToSphere < 0.001f)
        {
            float3 normal = normalize(currentPosition - sphereCenter);
            float3 lightDir = normalize(float3(1.0f, 1.0f, -1.0f));
            float lighting = max(dot(normal, lightDir), 0.2f);

            return float4(float3(0.0f, 0.5f, 1.0f) * lighting, 1.0f);
        }
        
        currentPosition += rayDirection * distanceToSphere;
    }

    // Miss: Background Color
    return float4(0.15f, 0.15f, 0.15f, 1.0f);
}