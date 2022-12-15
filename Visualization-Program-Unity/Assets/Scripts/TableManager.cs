using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public struct tick
{
    public string time;
    public string massIs;
    public string massLeft;
    public string vectv;
}
public class TableManager : MonoBehaviour
{
    public Queue<tick> que;

    private void Start()
    {
        UpdateDict();
        Debug.Log("OK");
    }

    public void UpdateDict()
    {
        que = new Queue<tick>();
        TextAsset table = Resources.Load<TextAsset>("data"); // name of table
        string[] lines = table.text.Split(new char[] { '\n' }, System.StringSplitOptions.RemoveEmptyEntries);
        for (int i = 0; i < lines.Length; i++)
        {
            string[] line = lines[i].Split(new char[] { ';' }, System.StringSplitOptions.RemoveEmptyEntries);
            tick t = new tick { time = line[0], massIs = line[1], massLeft = line[2], vectv=line[3] };
            que.Enqueue(t);
        }

    }

    public tick GetLine()
    {
        if (que.Count < 1) return new tick { time = "-1", massIs = "0", massLeft = "0", vectv = "0" }; ;
        return que.Dequeue();
    }

}
