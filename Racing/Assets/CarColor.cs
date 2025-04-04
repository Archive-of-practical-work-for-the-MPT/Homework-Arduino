using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CarColor : MonoBehaviour
{
    public Material CarMaterial;

    public Color StartColor;
    public Color EndColor;

    public float ChangeSpeed = 0.5f;

    private void FixedUpdate()
    {
        CarMaterial.color = Color.Lerp(StartColor, EndColor, Mathf.PingPong(Time.time, 1));
    }
}
