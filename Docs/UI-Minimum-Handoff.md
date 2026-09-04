# 최소 HUD 구현 및 인수인계

## 범위

- 체력바/수치, 무기 이름, 탄창/보유 탄약, 우측 중앙 반투명 미션 박스, 크로스헤어. 경과 시간 데이터는 유지하지만 화면에는 표시하지 않는다.
- `UPriestUIManager`: 로컬 플레이어마다 자동 생성되는 LocalPlayerSubsystem.
- `FPriestHUDData`: 게임 로직이 UI에 전달하는 전체 표시 데이터.
- `UPriestHUDWidget`: 별도 WBP 에셋 없이 C++로 기본 화면 구성.
- `APriestHUDTestActor`: 게임 로직 없이 화면을 확인하는 개발용 액터.
- 맵, GameMode, PlayerController 및 입력 모드는 변경하지 않음.
- 메뉴, 결과 화면, 히트마커, 몬스터 체력바, 인벤토리는 후속 범위.

## 오늘 에디터에서 테스트하는 방법

1. 에디터를 종료한 상태에서 Development Editor / Win64로 빌드한다.
2. 에디터를 열고 테스트용 맵을 연다. 공용 맵 대신 별도 테스트 맵 사용을 권장한다.
3. 콘텐츠 브라우저에서 C++ Classes → ProjectPriest → UI → PriestHUDTestActor를 찾아 맵에 하나 배치한다. 필요하면 콘텐츠 브라우저 설정에서 C++ 클래스 표시를 켠다.
4. Play를 실행한다(Simulate가 아닌 플레이 모드).
5. 좌측 하단 HP, 우측 하단 무기/탄약, 우측 중앙 미션 박스, 중앙 조준점을 확인한다.
6. 1초마다 HP/탄약이 변하고, HP가 0까지 내려간 뒤 반복되는지 확인한다. 이 값은 실제 캐릭터 상태에 영향을 주지 않는다. 경과 시간은 테스트 데이터에서 계속 누적되어 HUD에 전달되지만 화면에는 표시되지 않는다.
7. 플레이 종료 후 액터의 Animate Data를 끄고 Test Data를 편집해 다시 Play한다. Max Health=0, 긴 미션 문구, 탄약=0 등을 확인한다.
8. 창 크기를 바꿔 배치가 유지되는지 확인한다. 최소 레이아웃이므로 아주 좁은 화면에서는 영역이 겹칠 수 있다.
9. Play를 종료하고 다시 시작해 HUD가 중복되지 않는지 확인한다.

실제 게임 연결 전에 테스트 액터를 제거한다. 테스트 액터는 매초 전체 데이터를 덮어쓴다. 한 명의 로컬 플레이어용 테스트 도구이며 멀티플레이 테스트 도구는 아니다.

## 블루프린트 연결

로컬 PlayerController의 BeginPlay에서:

1. Get Local Player Subsystem으로 PriestUIManager를 가져온다.
2. Make PriestHUDData에 초기 표시 값을 넣는다.
3. Show HUD를 호출한다. 반환값 false면 아직 로컬 컨트롤러 또는 화면을 사용할 수 없는 상태다.
4. 체력, 탄약, 목표 등이 변경될 때 최신 전체 구조체를 Update HUD로 전달한다.
5. 숨길 때 Hide HUD, 다시 표시할 때 최신 구조체로 Show HUD를 호출한다.

`UpdateHUD`는 생성/표시를 하지 않는다. 최초 생성은 `ShowHUD`로 한다.
구조체는 전체 스냅샷이므로 변경하지 않은 필드도 유지해서 전달해야 한다.
경과 시간은 UI가 계산하지 않는다. 게임 시간 담당자가 결정한 값을 전달한다.
리스폰 후에는 새 캐릭터의 데이터에 연결하고, 레벨 이동 후에는 새 PlayerController에서 ShowHUD를 호출한다.

## C++ 연결 예시

```cpp
#include "UI/PriestUIManager.h"
#include "Engine/LocalPlayer.h"

// 로컬 PlayerController 멤버 함수 내부
if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
{
    if (UPriestUIManager* UI = LocalPlayer->GetSubsystem<UPriestUIManager>())
    {
        FPriestHUDData Data;
        Data.Health = 75.0f;
        Data.MaxHealth = 100.0f;
        Data.WeaponName = FText::FromString(TEXT("Rifle"));
        Data.MagazineAmmo = 12;
        Data.ReserveAmmo = 90;
        Data.MissionObjective = FText::FromString(TEXT("Reach the checkpoint"));
        Data.ElapsedSeconds = 65.0f;
        UI->ShowHUD(Data);
    }
}
```

UI는 데미지, 탄약 소비, 시간 누적, 미션 성공 판정을 하지 않는다. HUD는 HitTestInvisible이며 마우스 입력을 가로채지 않는다.

## 공유할 파일

`Source/ProjectPriest/UI/`, `Source/ProjectPriest/ProjectPriest.Build.cs`, 이 문서를 커밋한다. 솔루션, Binaries, Intermediate는 공유하지 않는다. 테스트 맵을 공유하려면 별도로 저장하고 해당 에셋만 추가한다.

## 검증 구분

C++ 빌드 성공과 에디터에서 화면을 확인하는 것은 별개다. 위 Play 체크리스트는 제출 전 수동 확인이 필요하다.

