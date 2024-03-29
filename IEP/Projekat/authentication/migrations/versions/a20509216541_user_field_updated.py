"""User field updated

Revision ID: a20509216541
Revises: 
Create Date: 2023-06-18 19:25:27.653229

"""
from alembic import op
import sqlalchemy as sa
from sqlalchemy.dialects import mysql

# revision identifiers, used by Alembic.
revision = 'a20509216541'
down_revision = None
branch_labels = None
depends_on = None


def upgrade():
    # ### commands auto generated by Alembic - please adjust! ###
    with op.batch_alter_table('user', schema=None) as batch_op:
        batch_op.add_column(sa.Column('forename', sa.String(length=256), nullable=False))
        batch_op.add_column(sa.Column('surname', sa.String(length=256), nullable=False))
        batch_op.alter_column('email',
               existing_type=mysql.VARCHAR(length=40),
               type_=sa.String(length=256),
               existing_nullable=False)
        batch_op.alter_column('password',
               existing_type=mysql.VARCHAR(length=20),
               type_=sa.String(length=256),
               existing_nullable=False)
        batch_op.drop_column('first_name')
        batch_op.drop_column('last_name')

    # ### end Alembic commands ###


def downgrade():
    # ### commands auto generated by Alembic - please adjust! ###
    with op.batch_alter_table('user', schema=None) as batch_op:
        batch_op.add_column(sa.Column('last_name', mysql.VARCHAR(length=20), nullable=False))
        batch_op.add_column(sa.Column('first_name', mysql.VARCHAR(length=20), nullable=False))
        batch_op.alter_column('password',
               existing_type=sa.String(length=256),
               type_=mysql.VARCHAR(length=20),
               existing_nullable=False)
        batch_op.alter_column('email',
               existing_type=sa.String(length=256),
               type_=mysql.VARCHAR(length=40),
               existing_nullable=False)
        batch_op.drop_column('surname')
        batch_op.drop_column('forename')

    # ### end Alembic commands ###
