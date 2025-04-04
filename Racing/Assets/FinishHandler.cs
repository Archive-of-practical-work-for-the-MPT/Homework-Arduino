using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class FinishHandler : MonoBehaviour
{
    [SerializeField] private Text _timerTxt;

    private void OnTriggerEnter(Collider other)
    {
        Debug.Log("Вы финишировали! " + _timerTxt.text);
    }
}
