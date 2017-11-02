using System;

namespace CSharp
{
    class Program
    {
        private static MutirotorClient client = new MutirotorClient();
        public static void Main(string[] args)
        {
            client.CreateClient();
            client.ConfirmConnection();
            client.EnableApiControl();
            client.ArmDisarm(true);
            client.Takeoff(5);
            client.Sleep(5);
            client.Hover();
            client.EnableApiControl();
            client.MoveByVelocityZ(3);
            client.Sleep(5);
            client.MoveByVelocityZ(-3);
            client.Hover();
            client.Land();
            client.ArmDisarm(false);
            client.DisposeClient();
        }
    }
}
