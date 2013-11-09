#ifndef  __DEF_CLASS_POKER_CARD__H__
#define  __DEF_CLASS_POKER_CARD__H__

#include "CommonDef.h"
#include "CommonData.h"
#include "PokerCardCommonDef.h"

#include <algorithm>  

NS_BEGIN(TA_TwoToOnePokerGameNet)

class CPokerCard
{
public:
	CPokerCard();
	CPokerCard(PokerSuit nPokerSuit, PokerValue nPokerValue);
	~CPokerCard(void);

public:
	CPokerCard* m_pThis;

public:
	/// <summary>
	/// Card����
	/// </summary>
	std::string getName() const;
	void setName(std::string& strPokerName);
	void setName(PokerValue nPokerValue, PokerSuit nPokerSuit);


	/// <summary>
	/// ��ȡ�ƵĴ�С
	/// �Ƶ���ֵPokerValue
	/// </summary>
	PokerValue getValue() const;
	void setValue(PokerValue nPokerValue);

	
	//�Ƶ�Ȩֵ PokerIntValue 
	PokerIntValue getRealValue() const;
	void setRealValue(PokerValue nPokerValue);


	/// <summary>
	/// ��ȡ��ǰ�ƵĻ�ɫ
	/// </summary>
	PokerSuit getSuit() const;
	void setSuit(PokerSuit nPokerSuit);

	/// <summary>
	/// ��ȡ�������˿����Ƿ��泯��
	/// </summary>
	bool getIsFaceUp() const;
	void setIsFaceUp(bool bisFaceUp);

	/// <summary>
	/// ���Ƿ���������Ӧ
	/// </summary>
	bool getHasResponse() const;
	void setHasResponse(bool bHasResponse);

	/// <summary>
	/// ��ȡ�������Ƶĳ��� ��ѡ����ʱ������仯����
	/// </summary>
	PokerUpDirection getUpDirectione() const;
	void setUpDirection(PokerUpDirection nUpDirection);

	//CanSelect����:bool
	bool getCanSelect() const;
	void setCanSelect(bool bCanSelect);

	//���Ƿ�ѡ��
	bool getIsSelected() const;
	void setIsSelected(bool bIsSelected);

	Visibility getVisibility() const;
	void setVisibility(Visibility nVisibility);
	
	//��һ���ճ�ʼ�õ����е�λ��
	int getIndexInInitList() const;
	void setIndexInInitList(int nIndexInInitList) ;

	//��һ����ϴ�����е�λ��
	int getIndexInDeckList() const;
	void setIndexInDeckList(int nIndexInDeckList);

public:
	/// <summary>
	/// �ƶ��Ƶķ���
	/// </summary>
	/// <param name="offsetX">Ҫ�ƶ���Xƫ����</param>
	/// <param name="offsetY">Ҫ�ƶ���Yƫ����</param>
	void moveCard(double offsetX, double offsetY);

public:
	void printInfoToLog();
	const char* getStringPokerValue(PokerValue nPokerValue);
	const char* getStringPokerSuit(PokerSuit nPokerSuit);

private:
//�ܱ����ķ���
//�¼�����
	/// <summary>
	/// �������ʱ������ܹ�ѡ�У���ѡ�д�����
	/// </summary>
	/// <param name="e"></param>
	void _OnMouseLeftButtonDown();
	//ѡ�к�ȡ��ѡ�еķ���
	/// <summary>
	/// ѡ��һ���Ƶķ���
	/// </summary>
	void _SelectCard();
	/// <summary>
	/// ȡ��ѡ��һ���Ƶķ���
	/// </summary>
	void _UnSelectCard();


private:
	PokerSuit _suit;//���滨ɫ
	PokerValue _value;//����ֵ
	PokerIntValue _realValue;//�Ƶ�Ȩֵ
	PokerUpDirection _upDirection;//�Ƶĳ��� ��ѡ����ʱ������仯����
	Visibility _Visibility;

	double    _PokerWidth;//�˿˿�� 140
	double    _PokerHeight;//�˿˸߶� 100
	bool	  _isFaceUp;//true �˿����Ƿ��泯��
	bool      _hasResponse;//����������Ӧ
	bool	  _canSelect;//CanSelect����:bool
	bool	  _isSelected;//���Ƿ�ѡ��

	int       m_nIndexInInitList;//��һ���ճ�ʼ�õ����е�λ��
	int       m_nIndexInDeckList;//��һ����ϴ�����е�λ��

private:
	std::string  m_strName;//Card����


public:
	bool CPokerCard::operator < (const CPokerCard& rhs) const;
public:
	CPokerCard& CPokerCard::operator =(const CPokerCard& rhs);//�������캯��


};

NS_END(TA_TwoToOnePokerGameNet)

#endif //__DEF_CLASS_POKER_CARD__H__
