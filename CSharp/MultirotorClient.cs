using System;
using System.Runtime.InteropServices;

namespace CSharp
{
    public class MutirotorClient
    {

        private IntPtr client;
        
        [DllImport("HelloDrone.dll")]
        private static extern IntPtr createClient();
        
        public void CreateClient() => client = createClient();


        [DllImport("HelloDrone.dll")]
        private static extern void disposeClient(IntPtr ptr);

        // TODO make disposable
        public void DisposeClient() => disposeClient(client);


        [DllImport("HelloDrone.dll")]
        private static extern void confirmConnection(IntPtr ptr);

        public void ConfirmConnection() => confirmConnection(client);


        [DllImport("HelloDrone.dll")]
        private static extern void enableApiControl(IntPtr ptr);

        public void EnableApiControl() => enableApiControl(client);


        [DllImport("HelloDrone.dll")]
        private static extern void armDisarm(IntPtr ptr, bool isArm);

        public void ArmDisarm(bool isArm) => armDisarm(client, isArm);
        

        [DllImport("HelloDrone.dll")]
        private static extern void takeoff(IntPtr ptr, float timeout);

        public void Takeoff(float timeout) => takeoff(client, timeout);


        [DllImport("HelloDrone.dll")]
        private static extern void sleepClient(IntPtr ptr, float time);

        public void Sleep(float time) => sleepClient(client, time);
        

        [DllImport("HelloDrone.dll")]
        private static extern void hover(IntPtr ptr);

        public void Hover() => hover(client);


        [DllImport("HelloDrone.dll")]
        private static extern void moveByVelocityZ(IntPtr ptr, float speed);

        public void MoveByVelocityZ(float speed) => moveByVelocityZ(client, speed);


        [DllImport("HelloDrone.dll")]
        private static extern void land(IntPtr ptr);

        public void Land() => land(client);
    }
}
