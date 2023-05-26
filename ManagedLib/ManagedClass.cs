using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ManagedLib
{
    public class ManagedClass
    {        
        public static int ManagedMethod(String pwzArgument)
        {
            Console.WriteLine("Inside ManagedClass.ManagedMethod()");
            return 1;
        }
    }
}
