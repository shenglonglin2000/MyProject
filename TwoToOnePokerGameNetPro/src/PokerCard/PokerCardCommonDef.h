///////////////////////////////////////////////////////////
//  PokerCardCommonDef.h
//  Implementation of the Class CProFunRes
//  Created on:      20-2-2012 09:31:53
//  Original author: Shenglong.Lin
//  #include 
//  #define
//  enum
//  typedef
//  static const char*
///////////////////////////////////////////////////////////



#ifndef  __DEF__POKER_CARD_COMMON_DEF_H__
#define  __DEF__POKER_CARD_COMMON_DEF_H__


NS_BEGIN(TA_TwoToOnePokerGameNet)

/*�˿˿�� DEF_INT_POKER_WIDTH    140*/
#define  DEF_INT_POKER_WIDTH    140

/*�˿˸߶� DEF_INT_POKER_HEIGHT   100*/
#define  DEF_INT_POKER_HEIGHT   100  

//���滨ɫ
//:S(����,spade)��C(�ݻ�,club)��H(����,heart)��D(��Ƭ,diamond)
//���ֻ�ɫ����four suits/
//�ֱ���/
//����:spade
//����:heart
//��Ƭ:diamond
//�ݻ�:club
typedef enum en_PokerSuit
{
	Club = 0,//�ݻ�
	Spader = 1,//����
	Heart = 2,  //����
	Diamond = 3,//��Ƭ
	Other = 4,
}PokerSuit;



struct PokerSuitNameValue
{
	const char*   pPokerSuitName;
	PokerSuit     nPokerSuit;
};

//���淽��
typedef enum en_PokerUpDirection
{
	Up,
	Left,
	Right,
	Down,
}PokerUpDirection;

//����ֵ
typedef enum en_PokerValue
{
	Three = 3,
	Four = 4,
	Five = 5,
	Six = 6,
	Seven = 7,
	Eight = 8,
	Nine = 9,
	Ten = 10,
	Jack = 11,
	Queen = 12,
	King = 13,
	Ace = 1,
	Two = 2,
	SmallJoker = 14,
	BigJoker = 15,
}PokerValue;

struct PokerValueNameValue
{
	const char*   pPokerValueName;
	PokerValue    nPokerValue;
};

//�Ƶ���ֵPokerValue
//�Ƶ�Ȩֵ PokerIntValue 
//if (PokerValue <= 13) {PokerIntValue = (PokerValue + 10) % 13 + 1;}
typedef enum en_PokerIntValue
{
	Poker_Three_IntValue = 1,
	Poker_Four_IntValue = 2,
	Poker_Five_IntValue = 3,
	Poker_Six_IntValue = 4,
	Poker_Seven_IntValue = 5,
	Poker_Eight_IntValue = 6,
	Poker_Nine_IntValue = 7,
	Poker_Ten_IntValue = 8,
	Poker_Jack_IntValue = 9,
	Poker_Queen_IntValue = 10,
	Poker_King_IntValue = 11,
	Poker_Ace_IntValue = 12,
	Poker_Two_IntValue = 13,
	Poker_SmallJoker_IntValue = 14,
	Poker_BigJoker_IntValue = 15,
}PokerIntValue;


typedef enum en_PlayerType
{
	CPU = 0, 
	Human = 1,
}PlayerType;


typedef enum en_PlayerPosition
{
	South = 0,		
	East = 1,
	North = 2,
	West = 3	
}PlayerPosition;

struct PlayerPositionNameValue
{
	const char*      pPlayerPositionName;
	PlayerPosition   nPlayerPosition;
};

typedef enum en_CardsStyle
{
	Single,   //����
	Double,   //����
	Trible,   //����
	Pass,   //δ����
	Bomb,   //ը��
	Rocket,  //���
	ThreeAndOne, //����һ
	ThreeAndTwo, //������
	SimpleConnectors,  //��˳  
	DoubleConnectors,  //˫˳
	TribleConnectors,  //��˳
	FourAndTwo,  //�Ĵ���
	Plane,   //�ɻ������
	Error     //δ֪����
}CardsStyle;

struct CardsStyleNameValue
{
	const char*   pCardsStyleName;
	CardsStyle    nCardsStyle;
};

typedef enum en_GameStatus
{
	NewGame,  //��Ϸ�տ�ʼ
	CleanEnv,//������Ϸ���������������ϵ��ƣ�3������뿪��λ��
	ThreePlayersSitDown,//3���������λ��
	PreparePokerCards,//׼��һ���µ��˿���
	ShufflePokerCards,//ϴ��
	DealPokerCards,//����
	DealedCardsWithNoSort, //�ַ���51���Ƶ�û����
	DealedCardsWithSort, //�ַ���51���Ʋ��Ѿ��������
	LordElected,  //����ѡ�����
	DealedCardsToLordWithNoSort,  //�����������ѷ�
	WaitingForPlayerToPlayCards, //׼���µ�һ��Player����
	CardsPlayedWithNoSort,   //���Ѿ����ˣ���û������
	CardsPlayedWithSort,     //���Ѿ����ˣ������Ѿ�������
	CongratulationWinner,
	GamdOver, //��Ϸ�Ѿ�����
	ExitGame,//�˳���Ϸ
}GameStatus;

typedef enum en_Visibility
{
	// Summary:
	//     Display the element.
	Visible = 0,
	//
	// Summary:
	//     Do not display the element, but reserve space for the element in layout.
	Hidden = 1,
	//
	// Summary:
	//     Do not display the element, and do not reserve space for it in layout.
	Collapsed = 2,
}Visibility;


NS_END(TA_TwoToOnePokerGameNet)

#endif  // __DEF__POKER_CARD_COMMON_DEF_H__











