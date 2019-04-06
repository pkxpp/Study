using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace tutorial3
{
    class Program
    {
        static FirstAgent g_FirstAgent;
        static SecondAgent g_SecondAgent;
        static SecondAgent g_ThirdAgent;
        const string strBehaviorTree = "InstanceBT";//"LoopBT";

        static bool InitBehavic()
        {
            Console.WriteLine("InitBehavic");

            behaviac.Workspace.Instance.FilePath = "../../exported";
            behaviac.Workspace.Instance.FileFormat = behaviac.Workspace.EFileFormat.EFF_xml;

            return true;
        }

        static bool InitPlayer()
        {
            Console.WriteLine("InitPlayer");

            // 创建g_FirstAgent，并加载行为树“InstanceBT”
            g_FirstAgent = new FirstAgent();
            bool bRet = g_FirstAgent.btload(strBehaviorTree);
            //Debug.Assert(bRet);
            g_FirstAgent.btsetcurrent(strBehaviorTree);

            // 创建g_SecondAgent，并将该实例赋给g_FirstAgent的成员pInstance
            g_SecondAgent = new SecondAgent();
            g_FirstAgent._set_pInstance(g_SecondAgent);

            // 创建g_ThirdAgent，并将"SecondAgentInstance"绑定给该实例
            g_ThirdAgent = new SecondAgent();
            behaviac.Agent.BindInstance(g_ThirdAgent, "SecondAgentInstance");

            return bRet;
        }

        static void UpdateLoop()
        {
            Console.WriteLine("UpdateLoop");

            int frames = 0;
            behaviac.EBTStatus status = behaviac.EBTStatus.BT_RUNNING;

            while (status == behaviac.EBTStatus.BT_RUNNING)
            {
                Console.WriteLine("frame {0}", ++frames);

                status = g_FirstAgent.btexec();
            }
        }

        static void CleanupPlayer()
        {
            Console.WriteLine("CleanupPlayer");

            g_FirstAgent = null;
        }

        static void CleanupBehaviac()
        {
            Console.WriteLine("CleanupBehaviac");

            behaviac.Workspace.Instance.Cleanup();
        }

        static void Main(string[] args)
        {
            InitBehavic();

            InitPlayer();

            UpdateLoop();

            CleanupPlayer();

            CleanupBehaviac();

            Console.Read();
        }
    }
}
