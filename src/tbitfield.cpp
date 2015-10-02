// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

const TELEM gMask[] = {0x1,    0x2,    0x4,    0x8,
                       0x10,   0x20,   0x40,   0x80,
                       0x100,  0x200,  0x400,  0x800,
                       0x1000, 0x2000, 0x4000, 0x8000,
                       0x10000, 0x20000, 0x40000, 0x80000,
                       0x100000, 0x200000, 0x400000, 0x800000,
                       0x1000000, 0x2000000, 0x4000000, 0x8000000,
                       0x10000000, 0x20000000, 0x40000000, 0x80000000};

int TelemBitSize() {
  return sizeof(TELEM) * 8;
}

TBitField::TBitField(int len) {
  if (len < 0)
    throw std::exception("TBitField(len): len should be >= 0");

  int tm = TelemBitSize();
  BitLen = len;
  MemLen = len / TelemBitSize() + (len % TelemBitSize() == 0 ? 0 : 1);
  pMem = new TELEM[MemLen];
  memset(pMem, 0, sizeof(TELEM) * MemLen);

  if (pMem == 0)
    throw std::exception("TBitField(len): out of memory");
}

TBitField::TBitField(const TBitField &bf) { // конструктор копирования
  BitLen = bf.BitLen;
  MemLen = bf.MemLen;
  pMem = new TELEM[MemLen];
  if (pMem == 0) throw std::exception("TBitField(len): out of memory");
  for (int i = 0; i < MemLen; ++i)
    pMem[i] = bf.pMem[i];
}

TBitField::~TBitField() {
  delete[] pMem;
  pMem = 0;
}

int TBitField::GetMemIndex(const int n) const { // индекс Мем для бита n
  if (n >= BitLen || n < 0) throw std::exception("TBitField::GetMemIndex(const int n): out of bitfield");
  return n / TelemBitSize();
}

TELEM TBitField::GetMemMask(const int n) const { // битовая маска для бита n
  if (n >= BitLen || n < 0) throw std::exception("TBitField::GetMemIndex(const int n): out of bitfield");
  return gMask[n % TelemBitSize()];
}

// доступ к битам битового поля

int TBitField::GetLength(void) const { // получить длину (к-во битов)
  return BitLen;
}

void TBitField::SetBit(const int n) { // установить бит
  if (n >= BitLen || n < 0) throw std::exception("TBitField::SetBit(const int n): out of bitfield");
  pMem[GetMemIndex(n)] |= gMask[n % TelemBitSize()];
}

void TBitField::ClrBit(const int n) { // очистить бит
  if (n >= BitLen || n < 0) throw std::exception("TBitField::ClrBit(const int n): out of bitfield");
  pMem[GetMemIndex(n)] &= ~gMask[n % TelemBitSize()];
}

int TBitField::GetBit(const int n) const { // получить значение бита
  if (n >= BitLen || n < 0) throw std::exception("TBitField::GetBit(const int n): out of bitfield");
  if ( (pMem[GetMemIndex(n)] & gMask[n % TelemBitSize()]) == 0) {
    return 0;
  } else {
    return 1;
  }
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) { // присваивание
  delete[] pMem;
  BitLen = bf.BitLen;
  MemLen = bf.MemLen;
  pMem = new TELEM[MemLen];
  if (pMem == 0) throw std::exception("TBitField(len): out of memory");
  for (int i = 0; i < MemLen; ++i)
    pMem[i] = bf.pMem[i];
  return *this;
}

int TBitField::operator==(const TBitField &bf) const { // сравнение
  if (BitLen != bf.BitLen) return 0;
  for (int i = 0; i < MemLen; ++i)
    if (pMem[i] != bf.pMem[i]) return 0;

  return 1;
}

int TBitField::operator!=(const TBitField &bf) const { // сравнение
  if (*this == bf) {
    return 0;
  } else {
    return 1;
  }
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
  int min_bit_len = bf.BitLen < BitLen ? bf.BitLen : BitLen;
  int max_bit_len = bf.BitLen > BitLen ? bf.BitLen : BitLen;
  int min_elem_len = bf.MemLen < MemLen ? bf.MemLen : MemLen;
  int max_elem_len = bf.MemLen > MemLen ? bf.MemLen : MemLen;

  TBitField new_bf(max_bit_len);
  for (int i = 0; i < min_elem_len; ++i)
    new_bf.pMem[i] = pMem[i] | bf.pMem[i];
  if (bf.MemLen > MemLen) {
    for (int i = min_elem_len; i < max_elem_len; ++i)
      new_bf.pMem[i] = bf.pMem[i];
  } else {
    for (int i = min_elem_len; i < max_elem_len; ++i)
      new_bf.pMem[i] = bf.pMem[i];
  }
  
  return new_bf;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
  int min_bit_len = bf.BitLen < BitLen ? bf.BitLen : BitLen;
  int max_bit_len = bf.BitLen > BitLen ? bf.BitLen : BitLen;
  int min_elem_len = bf.MemLen < MemLen ? bf.MemLen : MemLen;
  int max_elem_len = bf.MemLen > MemLen ? bf.MemLen : MemLen;

  TBitField new_bf(max_bit_len);
  for (int i = 0; i < min_elem_len; ++i)
    new_bf.pMem[i] = pMem[i] & bf.pMem[i];
  for (int i = min_elem_len; i < max_elem_len; ++i)
    new_bf.pMem[i] = 0;

  return new_bf;
}

TBitField TBitField::operator~(void) // отрицание
{
  TBitField new_bf(*this);

  for (int i = 0; i < MemLen; ++i)
    new_bf.pMem[i] = ~new_bf.pMem[i];

  if (MemLen > 0)
    for (int i = BitLen % TelemBitSize(); i < TelemBitSize(); ++i) {
      TELEM tmp = ~gMask[i];
      new_bf.pMem[MemLen - 1] &= ~gMask[i];
    }

  return new_bf;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
  return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
  return ostr;
}
