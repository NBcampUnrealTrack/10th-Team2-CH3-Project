#include "BTTask_PrintLog.h"
#include "JUtility.h"
UBTTask_PrintLog::UBTTask_PrintLog()
{
}

EBTNodeResult::Type UBTTask_PrintLog::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    JLog("%s", *Message);

    return EBTNodeResult::Succeeded;
}
