# Write your code here
import random
import sys
import sqlite3


class Account:
    dbcon = sqlite3.connect('card.s3db')
    dbcur = dbcon.cursor()

    def __init__(self):
        self.card_no = None
        self.pin = None
        self.balance = 0

    @staticmethod
    def create_table():
        Account.dbcur.execute('''
                            Create table if not exists "card"(
                            ID INTEGER IDENTITY (1,1),
                            NUMBER TEXT,
                            PIN TEXT,
                            BALANCE INTEGER
                            )
                                ''')
        Account.dbcon.commit()

    def create_account(self):
        self.card_no = Account.generate_card_number()
        self.pin = ''.join(random.sample([str(n) for n in range(10)], 4))
        Account.dbcur.execute('Insert into card(NUMBER, PIN, BALANCE) values(?, ?, ?)', (self.card_no, self.pin, 0))
        Account.dbcon.commit()

    @staticmethod
    def generate_card_number():
        card_no = "400000"
        card_no += ''.join(random.sample([str(n) for n in range(10)], 9))
        total = 0
        for i, n in enumerate(card_no):
            n = int(n)
            if i % 2 == 0:
                n *= 2
            if n > 9:
                n -= 9
            total += n
        card_no += '0' if total % 10 == 0 else str(10 - (total % 10))
        return card_no

    @staticmethod
    def get_balance(cnum):
        Account.dbcur.execute('select BALANCE from card where NUMBER = ?', (cnum,))
        return Account.dbcur.fetchone()[0]

    @staticmethod
    def is_valid_account(cnum, pinno):
        Account.dbcur.execute('Select NUMBER from card where NUMBER =? and PIN=?', (cnum,pinno))
        return len(Account.dbcur.fetchall()) != 0

    @staticmethod
    def does_card_exist(cnum):
        Account.dbcur.execute(f'Select NUMBER FROM card where number ={cnum}')
        Account.dbcon.commit()
        return len(Account.dbcur.fetchall()) != 0

    @staticmethod
    def is_valid_card(cnum):
        is_valid = True
        if cnum[:6] == '400000':
            total = 0
            for i, n in enumerate(cnum[:-1]):
                n = int(n)
                if i % 2 == 0:
                    n *= 2
                if n > 9:
                    n -= 9
                total += n
            total += int(cnum[-1])
            if total % 10 != 0:
                is_valid = False
        else:
            is_valid = False
        return is_valid

    @staticmethod
    def add_income(cardnum, money):
        Account.dbcur.execute(f"""
                                update card
                                set BALANCE=BALANCE+{money}
                                where NUMBER={cardnum}
                                """)
        Account.dbcon.commit()

    @staticmethod
    def has_enough_balance(cnum, money):
        Account.dbcur.execute(f'select BALANCE from card where number={cnum} and balance>={money}')
        return Account.dbcur.fetchone() is not None

    @staticmethod
    def close_account(cardnum):
        Account.dbcur.execute(f'DELETE FROM card where NUMBER={cardnum}')
        Account.dbcon.commit()

    @staticmethod
    def transfer_money(cardnum1, cardnum2, money):
        Account.add_income(cardnum1,0-money)
        Account.add_income(cardnum2, money)

def login():
    cardnumber = input('Enter your card number:\n')
    pin = input('Enter your PIN:\n')
    print()
    if Account.is_valid_account(cardnumber, pin):
        print('You have successfully logged in!\n')
        print('''1. Balance
2. Add income
3. Do transfer
4. Close account
5. Log out
0. Exit
''')
        while True:
            choice = int(input())
            print()
            if choice == 1:
                print(f'Balance: {Account.get_balance(cardnumber)}\n')
            elif choice == 2:
                income = int(input('Enter income:\n'))
                Account.add_income(cardnumber,income)
                print('Income was added!')
            elif choice == 3:
                print('Transfer')
                card2 = input('Enter card number:\n')
                if card2 != cardnumber and Account.is_valid_card(card2):
                    if Account.does_card_exist(card2):
                        money = int(input('Enter how much money you want to transfer:\n'))
                        if Account.has_enough_balance(cardnumber, money):
                            Account.transfer_money(cardnumber, card2, money)
                            print('Success!')
                        else:
                            print('Not enough money!')
                    else:
                        print('Such a card does not exist')
                else:
                    print('Probably you made mistake in the card number. Please try again!')
            elif choice == 4:
                Account.close_account(cardnumber)
                print('The account has been closed!')
                break
            elif choice == 5:
                print('You have successfully logged out!')
                break
            elif choice == 0:
                print('Bye!')
                sys.exit()
    else:
        print('Wrong card number or PIN!')


def operation_menu():
    while True:
        print('''1. Create an account
2. Log into account
0. Exit
    ''')
        choice = int(input())
        print()
        if choice == 1:
            new_account = Account()
            new_account.create_account()
            print('Your card has been created')
            print(f'Your card number:\n{new_account.card_no}')
            print(f'Your card PIN:\n{new_account.pin}')
        elif choice == 2:
            login()
        else:
            print('Bye!')
            break


def main():
    Account.create_table()
    operation_menu()
    Account.dbcon.close()


if __name__ == '__main__':
    main()
