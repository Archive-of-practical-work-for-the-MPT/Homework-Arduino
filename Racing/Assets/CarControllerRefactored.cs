using System;
using System.Collections;
using System.Collections.Generic;
using System.Globalization;
using System.IO.Ports;
using System.Text.RegularExpressions;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class CarControllerRefactored : MonoBehaviour
{
    [Header("UI Elements")]
    [SerializeField] private Text _rpm;

    [Header("Wheel Settings")]
    [SerializeField] private WheelCollider[] _wheelColliders;
    [SerializeField] private Transform[] _wheelTransform;
    [SerializeField] private float _wheelAcceleration = 2f;

    [Header("Car Parameters")]
    [SerializeField] private float _motorForce = 2000f;
    [SerializeField] private float _breakForce = 3000f;
    [SerializeField] private float _maxSteerAngle = 25f;
    [SerializeField] private GameObject _centerOfMass;

    [Header("Drift Settings")]
    [SerializeField] private TrailRenderer[] _tireMarks;
    [SerializeField] private float _sidewaysRatio = 0.2f;
    [SerializeField] private int _score = 0;

    [Header("Arduino Connection")]
    [SerializeField] private string portName = "COM3";
    [SerializeField] private int baudRate = 9600;

    private float _horizontal;
    private float _currentSteerAngle, _currentbreakForce;
    private bool isBreaking;
    private bool isDrifting;
    private Rigidbody _rigidbody;
    private SerialPort serialPort;

    private void Start()
    {
        StartCoroutine(CalculateSpeed());

        _rigidbody = GetComponent<Rigidbody>();
        _rigidbody.centerOfMass = _centerOfMass.transform.localPosition;

        InitializeSerialPort();
    }

    private void InitializeSerialPort()
    {
        try
        {
            Debug.LogError("Доступные порты:");
            foreach (string port in SerialPort.GetPortNames())
            {
                Debug.LogError(port);
            }

            serialPort = new SerialPort(portName, baudRate);
            serialPort.ReadTimeout = 100;
            serialPort.Open();
            Debug.LogError("Порт открыт успешно!");
        }
        catch (System.Exception e)
        {
            Debug.LogError($"Ошибка: {e.GetType()} -> {e.Message}");
        }
    }

    private void OnDestroy()
    {
        if (serialPort != null && serialPort.IsOpen)
        {
            serialPort.Close();
            Debug.Log("Serial port closed");
        }
    }

    private void Update()
    {
        ReadArduinoInput();

        if (Input.GetKey(KeyCode.I))
        {
            SceneManager.LoadScene(SceneManager.GetActiveScene().buildIndex);
        }
    }

    private void ReadArduinoInput()
    {
        if (serialPort == null || !serialPort.IsOpen) return;

        try
        {
            string data = serialPort.ReadLine().Trim();
            data = Regex.Replace(data, @"[^0-9.,-]", ""); // Удаляем мусор
            string[] values = data.Split(',');

            if (values.Length != 3)
            {
                Debug.LogWarning($"Неверное кол-во значений: {data}");
                return;
            }

            // Парсинг оси X
            if (!float.TryParse(values[0], NumberStyles.Float, CultureInfo.InvariantCulture, out float rawX))
            {
                Debug.LogError($"Ошибка в X: {values[0]}");
                return;
            }

            // Обработка оси X
            float deadzone = 0.1f;
            _horizontal = Mathf.Abs(rawX) > deadzone ? rawX : 0f;
            _horizontal = Mathf.Clamp(_horizontal, -1f, 1f);

            // Парсинг оси Y
            if (!float.TryParse(values[1], NumberStyles.Float, CultureInfo.InvariantCulture, out float vertical))
            {
                Debug.LogError($"Ошибка в Y: {values[1]}");
                return;
            }
            vertical = Mathf.Clamp(vertical, -1f, 1f);
            _motorForce = Mathf.Lerp(0, 5000f, (vertical + 1f) / 2f);

            // Парсинг кнопки
            if (!int.TryParse(values[2], out int button))
            {
                Debug.LogError($"Ошибка в кнопке: {values[2]}");
                return;
            }
            isBreaking = button == 1;

            Debug.Log($"Успех: X={rawX}, Y={vertical}, B={button}");
        }
        catch (TimeoutException) { }
        catch (Exception e)
        {
            Debug.LogError($"Ошибка: {e.Message}");
        }
    }

    private void FixedUpdate()
    {
        HandleMotor();
        HandleSteering();
        UpdateWheels();
        IsDrifting();
    }

    public IEnumerator CalculateSpeed()
    {
        while (true)
        {
            float speed = 3.14f * (_wheelColliders[2].rpm / 60) * (_wheelColliders[3].radius * 2);
            speed *= 3.6f;
            _rpm.text = $"Скорость: {Convert.ToInt32(speed)} км/ч\nДрифт: {isDrifting}\nОчки: {_score}";
            yield return new WaitForSeconds(0.05f);
        }
    }

    private void HandleMotor()
    {
        float targetTorque = _motorForce * (isBreaking ? 0.1f : 1f);

        _wheelColliders[2].motorTorque = Mathf.Lerp(
            _wheelColliders[2].motorTorque,
            targetTorque,
            Time.deltaTime * _wheelAcceleration
        );

        _wheelColliders[3].motorTorque = Mathf.Lerp(
            _wheelColliders[3].motorTorque,
            targetTorque,
            Time.deltaTime * _wheelAcceleration
        );

        _currentbreakForce = isBreaking ? _breakForce : 0f;
        ApplyBreaking();
    }

    private void ApplyBreaking()
    {
        _wheelColliders[2].brakeTorque = _currentbreakForce;
        _wheelColliders[3].brakeTorque = _currentbreakForce;

        foreach (TrailRenderer T in _tireMarks)
        {
            T.emitting = isDrifting;
        }
    }

    private void HandleSteering()
    {
        _currentSteerAngle = _maxSteerAngle * _horizontal;
        _wheelColliders[0].steerAngle = _currentSteerAngle;
        _wheelColliders[1].steerAngle = _currentSteerAngle;
    }

    private void UpdateWheels()
    {
        for (int i = 0; i < 4; i++)
        {
            UpdateSingleWheel(_wheelColliders[i], _wheelTransform[i]);
        }
    }

    private void UpdateSingleWheel(WheelCollider wheelCollider, Transform wheelTransform)
    {
        wheelCollider.GetWorldPose(out Vector3 pos, out Quaternion rot);
        wheelTransform.rotation = rot;
        wheelTransform.position = pos;
    }

    private void IsDrifting()
    {
        _wheelColliders[2].GetGroundHit(out WheelHit WheelHit);
        float SidewaysLeftSlip = Mathf.Abs(WheelHit.sidewaysSlip);

        _wheelColliders[3].GetGroundHit(out WheelHit);
        float SidewaysRightSlip = Mathf.Abs(WheelHit.sidewaysSlip);

        float AverageSlip = (SidewaysLeftSlip + SidewaysRightSlip) / 2;

        if (AverageSlip > _sidewaysRatio)
        {
            isDrifting = true;
            _score++;
        }
        else
        {
            isDrifting = false;
        }
    }
}
