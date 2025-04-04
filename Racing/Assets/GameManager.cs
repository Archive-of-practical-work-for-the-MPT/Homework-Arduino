using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class GameManager : MonoBehaviour
{
    [SerializeField] private Text _timerTxt;

    private int _minutes, _seconds;

    private void Start()
    {
        _minutes = 0;
        _seconds = 0;
        StartCoroutine(Timer());
    }

    private IEnumerator Timer()
    {
        while(true)
        {
            yield return new WaitForSeconds(1f);
            _seconds++;
            if (_seconds == 60)
            {
                _minutes++;
                _seconds = 0;
            }

            if(_seconds < 10) 
                _timerTxt.text = _minutes.ToString() + ":0" + _seconds.ToString();
            else 
                _timerTxt.text = _minutes.ToString() + ":" + _seconds.ToString();
        }
    }
}
