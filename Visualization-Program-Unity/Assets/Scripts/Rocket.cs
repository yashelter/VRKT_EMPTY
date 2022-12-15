using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;

public class Rocket : MonoBehaviour
{
    public TextMeshProUGUI vectV;
    public TextMeshProUGUI massNow;
    public TextMeshProUGUI massWas;
    public TextMeshProUGUI mI;

    public TableManager tm;
    public bool go = false;

    public Transform rocket;

    public double speed = 0.2;
    private float timer = 0;

    void Start()
    {
        tm = FindObjectOfType<TableManager>(); 
    }

    void FixedUpdate()
    {
        if (go)
        {
            if (timer > speed)
            {
                tick t = tm.GetLine();
                if (t.time == "-1")
                {
                    go = false;
                }
                else
                {
                    vectV.SetText(t.vectv);
                    massNow.SetText(t.massLeft);
                    massWas.SetText(t.massIs);
                    mI.SetText(t.time);
                    rocket.Translate(5.69f / 1000f, 5.56f / 1000f, Time.deltaTime);
                }
                timer = 0;
             }
            else
            {
                timer += Time.deltaTime;
            }
        }
    }
    public void changeGo()
    {
        go = !go;
    }
    public void Restart()
    {
        tm.UpdateDict();
        rocket.position = new Vector2(0.8f, -3.61f);
    }
    public void IncreaseSpeed()
    {
        speed += 0.03;
    }
    public void DecreaseSpeed()
    {
        speed -= 0.03;
        if (speed < 0) { speed = 0; }
    }
}
