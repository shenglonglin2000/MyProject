#include "PokerCard.h"
#include "CommonDef.h"
#include "CommonData.h"
#include "DebugUtil.h"
#include "PokerCardCommonDef.h"

NS_BEGIN(TA_TwoToOnePokerGameNet)


CPokerCard::CPokerCard()
{	
	_value = BigJoker;
	setRealValue(_value);	
	_suit = Other;
	_upDirection = Up;

	_PokerWidth = DEF_INT_POKER_WIDTH;
	_PokerHeight = DEF_INT_POKER_HEIGHT;
	_isFaceUp = true;
	_hasResponse = true;
	_canSelect = true;
	_isSelected = false;
	_Visibility = Visible;
	m_nIndexInInitList = -1;//��һ���ճ�ʼ�õ����е�λ��
	m_nIndexInDeckList = -1;//��һ����ϴ�����е�λ��

	m_strName.clear();
	setName(_value, _suit);

	m_pThis = NULL;
}

CPokerCard::CPokerCard(PokerSuit nPokerSuit, PokerValue nPokerValue)
{
	_value = nPokerValue;
	setRealValue(_value);
	_suit = nPokerSuit;
	_upDirection = Up;

	_PokerWidth = DEF_INT_POKER_WIDTH;
	_PokerHeight = DEF_INT_POKER_HEIGHT;
	_isFaceUp = true;
	_hasResponse = true;
	_canSelect = true;
	_isSelected = false;
	_Visibility = Visible;
	m_nIndexInInitList = -1;//��һ���ճ�ʼ�õ����е�λ��
	m_nIndexInDeckList = -1;//��һ����ϴ�����е�λ��

	m_strName.clear();
	setName(_value, _suit);
	m_pThis = NULL;
}


//<
bool CPokerCard::operator < (const CPokerCard& rhs) const
{
	bool bRes = true;

	if (this->_realValue == rhs.getRealValue())
	{
		bRes = this->_suit < rhs.getSuit();
	}
	else
	{
		bRes = this->_realValue < rhs.getRealValue();
	}

	return bRes;
}

//=
CPokerCard& CPokerCard::operator = (const CPokerCard& rhs)
{
	this->_value = rhs.getValue();
	this->_realValue = rhs.getRealValue();
	this->_suit = rhs.getSuit();

	this->_upDirection = rhs.getUpDirectione();
	this->_isFaceUp = rhs.getIsFaceUp();
	this->_hasResponse = rhs.getHasResponse();
	this->_canSelect = rhs.getCanSelect();
	this->_isSelected = rhs.getIsSelected();
	this->m_nIndexInInitList = rhs.getIndexInInitList();
	this->m_nIndexInDeckList = rhs.getIndexInDeckList();
	this->m_strName = rhs.getName();
	this->_Visibility = rhs.getVisibility();
	
	this->m_pThis = rhs.m_pThis;

	return *this;
}


CPokerCard::~CPokerCard(void)
{
	this->m_pThis = NULL;
}

/// <summary>
/// Card����
/// </summary>
std::string CPokerCard::getName() const
{
	return m_strName;
}
void CPokerCard::setName(std::string& strPokerName)
{
	m_strName = strPokerName;
}
void CPokerCard::setName(PokerValue nPokerValue, PokerSuit nPokerSuit)
{
	std::string strPokerCardValue = "";
	std::string strPokerCardSuit = "";
	std::ostringstream osTmp;

	strPokerCardValue = getStringPokerValue(_value);
	strPokerCardSuit = getStringPokerSuit(_suit);
	
	osTmp.clear();
	osTmp.str("");
	osTmp << "Poker_" << strPokerCardValue << "_" << strPokerCardSuit;

	m_strName = osTmp.str();
}

/// <summary>
/// ��ȡ�ƵĴ�С
/// </summary>
PokerValue CPokerCard::getValue() const
{
	return _value;
}
void CPokerCard::setValue(PokerValue nPokerValue)
{
	_value = nPokerValue;
	setRealValue(_value);
}


//�Ƶ���ֵPokerValue
//�Ƶ�Ȩֵ PokerIntValue 
//if (PokerValue <= 13) {PokerIntValue = (PokerValue + 10) % 13 + 1;}
PokerIntValue CPokerCard::getRealValue() const
{		
	return _realValue;
}

	
void CPokerCard::setRealValue(PokerValue nPokerValue)
{		
	//PokerValue King=13  Ten=10 Ace = 1
	if (nPokerValue <= 13)
	{
		_realValue = (PokerIntValue)((nPokerValue + 10) % 13 + 1);
	}
	else
	{
		_realValue = (PokerIntValue)nPokerValue;
	}
	
	return;
}



/// <summary>
/// ��ȡ��ǰ�ƵĻ�ɫ
/// </summary>
PokerSuit CPokerCard::getSuit() const
{
	return _suit;
}
void CPokerCard::setSuit(PokerSuit nPokerSuit)
{
	_suit = nPokerSuit;
}

/// <summary>
/// ��ȡ�������˿����Ƿ��泯��
/// </summary>
bool CPokerCard::getIsFaceUp() const
{
	return _isFaceUp;
}
void CPokerCard::setIsFaceUp(bool bisFaceUp)
{
	_isFaceUp = bisFaceUp;
}

/// <summary>
/// ���Ƿ���������Ӧ
/// </summary>
bool CPokerCard::getHasResponse() const
{
	return _hasResponse;
}
void CPokerCard::setHasResponse(bool bHasResponse)
{
	_hasResponse = bHasResponse;
}

/// <summary>
/// ��ȡ�������Ƶĳ��� ��ѡ����ʱ������仯����
/// </summary>
PokerUpDirection CPokerCard::getUpDirectione() const
{
	return _upDirection;
}
void CPokerCard::setUpDirection(PokerUpDirection nUpDirection)
{
	if (!_isSelected)
	{
		_upDirection = nUpDirection;
	}
}

//CanSelect����:bool
bool CPokerCard::getCanSelect() const
{
	return _canSelect;
}
void CPokerCard::setCanSelect(bool bCanSelect)
{
	_canSelect = bCanSelect;
}

//���Ƿ�ѡ��
bool CPokerCard::getIsSelected() const
{
	return _isSelected;
}
void CPokerCard::setIsSelected(bool bIsSelected)
{
	if (bIsSelected)
	{
		_SelectCard();
	}
	else
	{
		_UnSelectCard();
	}
}


Visibility CPokerCard::getVisibility() const
{
	return _Visibility;
}
void CPokerCard::setVisibility(Visibility nVisibility)
{
	_Visibility = nVisibility;
}


//��һ���ճ�ʼ�õ����е�λ��
int CPokerCard::getIndexInInitList() const
{
	return m_nIndexInInitList;
}
void CPokerCard::setIndexInInitList(int nIndexInInitList)
{
	m_nIndexInInitList = nIndexInInitList;
}

//��һ����ϴ�����е�λ��
int CPokerCard::getIndexInDeckList() const
{
	return m_nIndexInDeckList;
}
void CPokerCard::setIndexInDeckList(int nIndexInDeckList)
{
	m_nIndexInDeckList = nIndexInDeckList;
}


void CPokerCard::printInfoToLog()
{
	std::string strPokerCardValue = "";
	std::string strPokerCardSuit = "";

	strPokerCardValue = getStringPokerValue(_value);
	strPokerCardSuit = getStringPokerSuit(_suit);

	_LOG(SourceFLInfo, DebugDebug, "PokerCard [strName=%s][_value=%s][_suit=%s][_realValue=%d][nIndexInInitList=%d][nIndexInDeckList=%d]",
		m_strName.c_str(), strPokerCardValue.c_str(), strPokerCardSuit.c_str(), _realValue, m_nIndexInInitList, m_nIndexInDeckList);
}


/// <summary>
/// �ƶ��Ƶķ���
/// </summary>
/// <param name="offsetX">Ҫ�ƶ���Xƫ����</param>
/// <param name="offsetY">Ҫ�ƶ���Yƫ����</param>
void CPokerCard::moveCard(double offsetX, double offsetY)
{

}



//�ܱ����ķ���
//�¼�����
/// <summary>
/// �������ʱ������ܹ�ѡ�У���ѡ�д�����
/// </summary>
/// <param name="e"></param>
void CPokerCard::_OnMouseLeftButtonDown()
{
	if (_canSelect)
	{
		if (!_isSelected)
		{
			_SelectCard();
		}
		else
		{
			_UnSelectCard();
		}
	}
}
//ѡ�к�ȡ��ѡ�еķ���
/// <summary>
/// ѡ��һ���Ƶķ���
/// </summary>
void CPokerCard::_SelectCard()
{
	//ȷ�ϵ�ǰû�б�ѡ�����ƿ��Ա�ѡ��
	if (!_isSelected && _canSelect)
	{
		switch (_upDirection)
		{
		case Up:
			moveCard(0, -30);
			break;
		case Down:
			moveCard(0, 30);
			break;
		case Left:
			moveCard(-30, 0);
			break;
		case Right:
			moveCard(30, 0);
			break;
		}
		_isSelected = true;
	}
}
/// <summary>
/// ȡ��ѡ��һ���Ƶķ���
/// </summary>
void CPokerCard::_UnSelectCard()
{
	//ȷ�ϵ�ǰ�Ѿ���ѡ��
	if (_isSelected)
	{
		//��ԭ��ԭ����λ��
		switch (_upDirection)
		{
		case Up:
			moveCard(0, 30);
			break;
		case Down:
			moveCard(0, -30);
			break;
		case Left:
			moveCard(30, 0);
			break;
		case Right:
			moveCard(-30, 0);
			break;
		}
		_isSelected = false;
	}
}


//////////////////////////////////////////////////////////////////////////

/*
struct PokerValueNameValue
{
const char*   pPokerValueName;
PokerValue    nPokerValue;
};
*/

static PokerValueNameValue nameValueMapping_PokerValueNameValue[] =
{
	{ "Ace",		Ace		},
	{ "Two",		Two		},
	{ "Three",		Three		},
	{ "Four",		Four		},
	{ "Five",		Five		},
	{ "Six",		Six		},
	{ "Seven",        Seven         },
	{ "Eight",      Eight       },
	{ "Nine",	    Nine	        },
	{ "Ten",	    Ten	    },
	{ "Jack",   Jack    },
	{ "Queen",	Queen	},
	{ "King",	King	},
	{ "SmallJoker",		SmallJoker		},
	{ "BigJoker",		BigJoker		},
	{ NULL,			BigJoker		}
};


const char* CPokerCard::getStringPokerValue(PokerValue nPokerValue)
{	
	PokerValueNameValue* nv = nameValueMapping_PokerValueNameValue;

	while (nv->pPokerValueName != NULL)
	{
		if (nPokerValue == nv->nPokerValue)
		{
			return nv->pPokerValueName;
		}
		nv++;
	}

	return "UNKNOWN";
}









//���滨ɫ
//:S(����,spade)��C(�ݻ�,club)��H(����,heart)��D(��Ƭ,diamond)
/*
struct PokerSuitNameValue
{
const char*   pPokerSuitName;
PokerSuit     nPokerSuit;
};
*/

static PokerSuitNameValue nameValueMapping_PokerSuitNameValue[] =
{
	{ "Club",		Club	 },
	{ "Spader",		Spader	 },
	{ "Heart",		Heart	 },
	{ "Diamond",	Diamond	 },
	{ "Other",		Other	 },
	{ NULL,			Other	 },
};


const char* CPokerCard::getStringPokerSuit(PokerSuit nPokerSuit)
{	
	PokerSuitNameValue* nv = nameValueMapping_PokerSuitNameValue;

	while (nv->pPokerSuitName != NULL)
	{
		if (nPokerSuit == nv->nPokerSuit)
		{
			return nv->pPokerSuitName;
		}
		nv++;
	}

	return "UNKNOWN";
}



NS_END(TA_TwoToOnePokerGameNet)





