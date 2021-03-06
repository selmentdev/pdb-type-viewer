#pragma once
#include <QAbstractItemModel>
#include <LibPtv/BaseTypeMember.hxx>
#include <LibPtv/TypeDescriptor.hxx>

namespace ptvapp::models
{
    enum class TypeDescriptorElementRole
    {
        Type,
        Name,
        Size,
        Offset,
        Kind,
        Bits,
        Padding,
        IsPadding,
        IsSpurious,
        CacheLine,
    };

    class TypeDescriptorElement
    {
    private:
        QList<TypeDescriptorElement*> m_Children;
        TypeDescriptorElement* m_Parent;
        LibPdb::BaseTypeMember* m_Member;

    public:
        explicit TypeDescriptorElement(
            LibPdb::BaseTypeMember* member,
            TypeDescriptorElement* parent = nullptr
        ) noexcept;

        ~TypeDescriptorElement() noexcept;

    public:
        void AppendChild(
            TypeDescriptorElement* child
        ) noexcept;

        TypeDescriptorElement* GetChild(
            int index
        ) const noexcept;

        int GetChildrenCount() const noexcept;

        QVariant data(
            TypeDescriptorElementRole role
        ) const noexcept;

        int GetRowIndex() const noexcept;

        TypeDescriptorElement* GetParent() const noexcept;

        void Clear() noexcept;

    public:
        LibPdb::BaseTypeMember* GetMember() const noexcept
        {
            return m_Member;
        }
    };
}

namespace ptvapp::models
{
    class TypeDescriptorModel : public QAbstractItemModel
    {
        Q_OBJECT
    private:
        TypeDescriptorElement* m_Root;
        std::unique_ptr<LibPdb::TypeDescriptor> m_CurrentDescriptor;

    public:
        explicit TypeDescriptorModel(
            QObject* parent = nullptr
        ) noexcept;

        ~TypeDescriptorModel() noexcept;

    public:
        void FromDescriptor(
            std::unique_ptr<LibPdb::TypeDescriptor>&& descriptor
        ) noexcept;

    private:
        void SetupModelData(
            const std::vector<std::unique_ptr<LibPdb::BaseTypeMember>>& members,
            TypeDescriptorElement* parent
        ) noexcept;

    public: // interface QAbstractItemModel
        virtual QVariant data(
            const QModelIndex& index,
            int role
        ) const override;

        virtual Qt::ItemFlags flags(
            const QModelIndex& index
        ) const override;

        virtual QVariant headerData(
            int section,
            Qt::Orientation orientation,
            int role = Qt::DisplayRole
        ) const override;

        virtual QModelIndex index(
            int row,
            int column,
            const QModelIndex& parent = {}
        ) const override;

        virtual QModelIndex parent(
            const QModelIndex& index
        ) const override;

        virtual int rowCount(
            const QModelIndex& parent = {}
        ) const override;

        virtual int columnCount(
            const QModelIndex& parent = {}
        ) const override;
    };
}
