using System;
using System.Runtime.CompilerServices;

public abstract class ScriptAPI
{
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    public extern static void DoSomething();
};

namespace TestNamespace
{
    public abstract class TestClass
    {
        public static void Print()
        {
            ScriptAPI.DoSomething();
        }
    }
}
;